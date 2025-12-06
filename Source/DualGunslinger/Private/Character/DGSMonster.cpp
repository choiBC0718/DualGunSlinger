// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DGSMonster.h"

#include "Bullet.h"
#include "DGSPlayer.h"
#include "MonsterFSM.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Framework/DGSGameMode.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/MonsterWidget.h"


ADGSMonster::ADGSMonster()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionProfileName("Enemy");

	FSM=CreateDefaultSubobject<UMonsterFSM>("FSM");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HPBarWidget=CreateDefaultSubobject<UWidgetComponent>("HPBarWidget");
	HPBarWidget->SetupAttachment(GetRootComponent());
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ADGSMonster::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ADGSMonster::MonsterOverlap);

	if (HPBarWidget)
	{
		UMonsterWidget* MonsterWidget = Cast<UMonsterWidget>(HPBarWidget->GetUserWidgetObject());
		if (MonsterWidget)
		{
			MonsterWidget->OwnerMonster=this;
			MonsterWidget->Init();
		}
	}
	CurPistolHP=MaxPistolHP;
	CurRifleHP=MaxRifleHP;
	bIsDead=false;
	OnHPChanged.Broadcast(CurPistolHP, CurRifleHP, MaxPistolHP, MaxRifleHP);

	AGameModeBase* CurGameMode = GetWorld()->GetAuthGameMode();
	if (CurGameMode)
	{
		GameMode=Cast<ADGSGameMode>(CurGameMode);
	}
}

void ADGSMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ADGSMonster::MonsterHit(EGunType GunType)
{
	if (FSM && FSM->mState == EEnemyState::Die)
		return;
	
	switch (MonsterType)
	{
	case EMonsterType::PistolWeak:
		if (GunType == EGunType::Pistol)
		{
			if (CurPistolHP>=1)
			{
				CurPistolHP--;
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX,GetActorLocation(),GetActorRotation());
				OnHPChanged.Broadcast(CurPistolHP, CurRifleHP, MaxPistolHP, MaxRifleHP);
			}
			if (CurPistolHP<=0)
			{
				bIsDead=true;
				if (GameMode)
				{
					GameMode->AddScore(10);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Immune"));
		}
		break;

	case EMonsterType::RifleWeak:
		if (GunType == EGunType::Rifle)
		{
			if (CurRifleHP>=1)
			{
				CurRifleHP--;
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX,GetActorLocation(),GetActorRotation());
				OnHPChanged.Broadcast(CurPistolHP, CurRifleHP, MaxPistolHP, MaxRifleHP);
			}
			if (CurRifleHP<=0)
			{
				bIsDead=true;
				if (GameMode)
				{
					GameMode->AddScore(10);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Immune"));
		}
		break;

	case EMonsterType::Hybrid:
		if (GunType==EGunType::Rifle)
		{
			if (CurRifleHP>=1)
			{
				CurRifleHP--;
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX,GetActorLocation(),GetActorRotation());
				OnHPChanged.Broadcast(CurPistolHP, CurRifleHP, MaxPistolHP, MaxRifleHP);
			}
		}
		else if (GunType==EGunType::Pistol)
		{
			if (CurPistolHP>=1)
			{
				CurPistolHP--;
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX,GetActorLocation(),GetActorRotation());
				OnHPChanged.Broadcast(CurPistolHP, CurRifleHP, MaxPistolHP, MaxRifleHP);
			}
		}

		if (CurPistolHP<=0 && CurRifleHP<=0)
		{
			bIsDead=true;
			if (GameMode)
			{
				GameMode->AddScore(30);
			}
		}
		break;
	}
	if (bIsDead)
	{
		if (FSM)
		{
			FSM->OnDamageProcess();
		}
	}
}

void ADGSMonster::MonsterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

