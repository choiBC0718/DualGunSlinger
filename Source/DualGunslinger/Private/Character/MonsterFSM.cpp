// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MonsterFSM.h"

#include "DGSMonster.h"
#include "DGSPlayer.h"
#include "Item.h"
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
	if (!Target)
		return;

	if (!Target->bIsDead)
	{
		CurrentTime += GetWorld()->DeltaTimeSeconds;
		if (CurrentTime > IdleDelayTime)
		{
			mState = EEnemyState::Move;
			CurrentTime = 0.f;
		}
	}
}

void UMonsterFSM::MoveState()
{
	if (!Target)
		return;
	if (Target->bIsDead)
	{
		mState=EEnemyState::Idle;
	}
	FVector Destination = Target->GetActorLocation();
	FVector Direction = Destination - Me->GetActorLocation();
	Me->AddMovementInput(Direction.GetSafeNormal(), 1.0f);

	if (Direction.Size() < AttackRange)
	{
		mState = EEnemyState::Attack;
	}
}

void UMonsterFSM::AttackState()
{
	if (!Target)
		return;
	if (Target->bIsDead)
		mState=EEnemyState::Idle;
	
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime>AttackDelayTime)
	{
		FVector Direction = Target->GetActorLocation() - Me->GetActorLocation();
		Me->SetActorRotation(Direction.Rotation());
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

	if (Target && FMath::RandRange(1,100) <= Target->ItemDropP)
	{
		if (Me->Items.Num()>0)
		{
			int32 RandIdx = FMath::RandRange(0,Me->Items.Num()-1);
			if (Me->Items[RandIdx])
			{
				GetWorld()->SpawnActor<AItem>(Me->Items[RandIdx],Me->GetActorLocation(), Me->GetActorRotation());
			}
		}
	}
}

