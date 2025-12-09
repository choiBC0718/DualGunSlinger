// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DGSPlayer.h"

#include "Bullet.h"
#include "DGSMonster.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Item.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CapsuleComponent.h"
#include "DualGunslinger/DualGunslinger.h"
#include "Framework/DGSGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/GameOverWidget.h"
#include "Widget/MainWidget.h"

// Sets default values
ADGSPlayer::ADGSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComp=CreateDefaultSubobject<USpringArmComponent>("Spring Arm Component");
	SpringArmComp->SetupAttachment(GetRootComponent());
	
	SpringArmComp->TargetArmLength=300.f;
	SpringArmComp->SetRelativeRotation(FRotator(-50.f, 0.f,0.f));
	
	CameraComp=CreateDefaultSubobject<UCameraComponent>("Camera Component");
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	GetMesh()->SetupAttachment(GetRootComponent());
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bUseControllerRotationYaw=false;

	PistolMesh=CreateDefaultSubobject<USkeletalMeshComponent>("Pistol Mesh");
	PistolMesh->SetupAttachment(GetMesh(),FName("ik_hand_gun"));
	PistolMesh->SetVisibility(false);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempPistolMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapons/Pistol/Meshes/SKM_Pistol.SKM_Pistol'"));
	if (TempPistolMesh.Succeeded())
	{
		PistolMesh->SetSkeletalMesh(TempPistolMesh.Object);
	}

	RifleMesh=CreateDefaultSubobject<USkeletalMeshComponent>("Rifle Mesh");
	RifleMesh->SetupAttachment(GetMesh(),FName("ik_hand_gun"));
	RifleMesh->SetVisibility(true);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempRifleMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapons/Rifle/Meshes/SKM_Rifle.SKM_Rifle'"));
	if (TempRifleMesh.Succeeded())
	{
		RifleMesh->SetSkeletalMesh(TempRifleMesh.Object);
	}
}

// Called when the game starts or when spawned
void ADGSPlayer::BeginPlay()
{
	Super::BeginPlay();

	bIsDead=false;
	bIsRifleMode=true;
	CurrentHp=MaxHealth;
	
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->SetInputMode(FInputModeGameOnly());
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (SubSystem)
		{
			SubSystem->RemoveMappingContext(InputMappingContext);
			SubSystem->AddMappingContext(InputMappingContext,0);
		}
	}

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ADGSPlayer::PlayerOverlap);

	if (GameOverClass)
	{
		GameOverWidget=CreateWidget<UGameOverWidget>(GetWorld(),GameOverClass);
	}
}

// Called every frame
void ADGSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ShootTime += DeltaTime;
}

// Called to bind functionality to input
void ADGSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComp)
	{
		EnhancedInputComp->BindAction(IA_ChangeGun, ETriggerEvent::Triggered, this, &ADGSPlayer::ChangeGun);
		EnhancedInputComp->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ADGSPlayer::Move);
		EnhancedInputComp->BindAction(IA_Shoot, ETriggerEvent::Triggered, this, &ADGSPlayer::Shoot);
	}
}

void ADGSPlayer::ChangeGun()
{
	if (bIsRifleMode)
	{
		bIsRifleMode=false;
		RifleMesh->SetVisibility(false);
		PistolMesh->SetVisibility(true);
	}
	else
	{
		bIsRifleMode=true;
		RifleMesh->SetVisibility(true);
		PistolMesh->SetVisibility(false);
	}
}

void ADGSPlayer::Move(const FInputActionValue& InputActionValue)
{
	FVector2D InputVal = InputActionValue.Get<FVector2D>();
	InputVal.Normalize();
	
	AddMovementInput(GetActorForwardVector()*InputVal.X + GetActorRightVector()*InputVal.Y);
}


void ADGSPlayer::PlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AItem* Item = Cast<AItem>(OtherActor);
	if (Item)
	{
		switch (Item->ItemType)
		{
		case EItemType::WalkSpeedItem:
			MoveSpeed += 20.f;
			GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
			break;
		case EItemType::BulletSpeedItem:
			BulletSpeed += 25.f;
			break;
		case EItemType::SpawnRateItem:
			ShootRate += 0.1f;
			break;
		case EItemType::HealthItem:
			if (CurrentHp<MaxHealth)
				CurrentHp++;
			break;
		}
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ItemEatFX,GetActorLocation());
		UGameplayStatics::PlaySound2D(GetWorld(),ItemEatSound);
		
		OtherActor->Destroy();
	}
}

void ADGSPlayer::GetDamaged()
{
	if (bIsDead)
		return;
	
	CurrentHp--;
	OnHPChanged.Broadcast(CurrentHp,MaxHealth);
	if (CurrentHp <= 0)
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = 0.f;
		}
		bIsDead=true;
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());

		AGameModeBase* CurGameMode = GetWorld()->GetAuthGameMode();
		if (!CurGameMode)
			return;
		ADGSGameMode* GameMode = Cast<ADGSGameMode>(CurGameMode);
		if (!GameMode)
			return;
		int32 FinishScore=GameMode->GetScore();
		FString PlayTime = GameMode->MainWidget->PlayTime;
		GameOverWidget->InitGameOver(FinishScore,PlayTime);
		GameOverWidget->AddToViewport();
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(FInputModeUIOnly());
		}
	}
}

void ADGSPlayer::Shoot(const FInputActionValue& InputActionValue)
{
	FVector2D InputVal = InputActionValue.Get<FVector2D>();
	if (InputVal.IsNearlyZero())
		return;
	
	if (InputVal.Y>0)		GetMesh()->SetRelativeRotation(FRotator(0,0,0));
	else if (InputVal.Y<0)	GetMesh()->SetRelativeRotation(FRotator(0,-180,0));
	else if (InputVal.X>0)	GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
	else if (InputVal.X<0)	GetMesh()->SetRelativeRotation(FRotator(0,90,0));

	float ActualFireRate = FMath::Max(ShootRate,0.1f);
	float ShootDelay = 1.f / ActualFireRate;
	if (ShootTime < ShootDelay)
		return;
	
	USkeletalMeshComponent* CurrentGunMesh = bIsRifleMode ? RifleMesh : PistolMesh;
	if (CurrentGunMesh && CurrentGunMesh->DoesSocketExist("Muzzle"))
	{
		const FVector MuzzleLocation = CurrentGunMesh->GetSocketLocation("Muzzle");
		const FRotator MuzzleRotation = GetMesh()->GetComponentRotation() + FRotator(0.f,90.f,0.f);

		FTransform SpawnTrans(MuzzleRotation,MuzzleLocation);
		
		UClass* Bullet = bIsRifleMode ? RifleBulletClass : PistolBulletClass;
		if (!Bullet)
			return;
		
		ABullet* SpawnBullet = GetWorld()->SpawnActorDeferred<ABullet>(Bullet,SpawnTrans,this,this,ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (SpawnBullet)
		{
			SpawnBullet->BulletType = bIsRifleMode ? EGunType::Rifle : EGunType::Pistol;
			SpawnBullet->MoveSpeed = BulletSpeed;
			SpawnBullet->MovementComp->InitialSpeed = BulletSpeed;
			SpawnBullet->MovementComp->MaxSpeed = BulletSpeed;

			UGameplayStatics::FinishSpawningActor(SpawnBullet,SpawnTrans);
			ShootTime = 0.f;
		}
	}
}

