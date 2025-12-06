// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MonsterFSM.h"

#include "DGSMonster.h"
#include "DGSPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMonsterFSM::UMonsterFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UMonsterFSM::IdleState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime > IdleDelayTime)
	{
		mState = EEnemyState::Move;
		CurrentTime = 0.f;
	}
}

void UMonsterFSM::MoveState()
{
	if (!Target)
		return;
	
	FVector Destination = Target->GetActorLocation();
	FVector Direction = Destination - Me->GetActorLocation();
	Me->AddMovementInput(Direction.GetSafeNormal(),MoveSpeedRate);

	if (Direction.Size() < AttackRange)
	{
		mState = EEnemyState::Attack;
	}
}

void UMonsterFSM::AttackState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime>AttackDelayTime)
	{
		CurrentTime = 0.f;
		Target->GetDamaged();
		if ((Target->GetActorLocation() - Me->GetActorLocation()).Size() > AttackRange)
		{
			mState=EEnemyState::Move;
		}
	}
}


void UMonsterFSM::DestroyOnLevel()
{
	Me->Destroy();
}

void UMonsterFSM::DieState()
{
	Me->SetActorLocation(Me->GetActorLocation() + FVector::DownVector*DieSpeed*GetWorld()->DeltaTimeSeconds);
}

// Called when the game starts
void UMonsterFSM::BeginPlay()
{
	Super::BeginPlay();

	auto Actor =UGameplayStatics::GetActorOfClass(GetWorld(), ADGSPlayer::StaticClass()); 
	Target=Cast<ADGSPlayer>(Actor);
	Me=Cast<ADGSMonster>(GetOwner());
}


// Called every frame
void UMonsterFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
}

void UMonsterFSM::OnDamageProcess()
{
	mState=EEnemyState::Die;
	Me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Me->SetActorLocation(Me->GetActorLocation()+FVector(0.f,0.f,DieHeight));
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle,this, &UMonsterFSM::DestroyOnLevel,5.f);
}

