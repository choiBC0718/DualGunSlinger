// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DGSMonster.h"

#include "Bullet.h"
#include "DGSPlayer.h"
#include "MonsterFSM.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ADGSMonster::ADGSMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionProfileName("Enemy");

	FSM=CreateDefaultSubobject<UMonsterFSM>("FSM");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ADGSMonster::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ADGSMonster::MonsterOverlap);
	CurPistolHP=MaxPistolHP;
	CurRifleHP=MaxRifleHP;
	bIsDead=false;
}

// Called every frame
void ADGSMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADGSMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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
			CurPistolHP--;
			if (CurPistolHP<=0)
				bIsDead=true;
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Immune"));
		}
		break;

	case EMonsterType::RifleWeak:
		if (GunType == EGunType::Rifle)
		{
			CurRifleHP--;
			if (CurRifleHP<=0)
				bIsDead=true;
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Immune"));
		}
		break;

	case EMonsterType::Hybrid:
		if (GunType==EGunType::Rifle)
		{
			CurRifleHP--;
		}
		else if (GunType==EGunType::Pistol)
		{
			CurPistolHP--;
		}

		if (CurPistolHP<=0 && CurRifleHP<=0)
			bIsDead=true;
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
	ABullet* Bullet = Cast<ABullet>(OtherActor);
	if (Bullet)
	{
		MonsterHit(Bullet->BulletType);
	}
}

