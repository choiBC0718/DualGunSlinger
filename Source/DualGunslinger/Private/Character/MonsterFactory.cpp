// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MonsterFactory.h"

#include "DGSMonster.h"

// Sets default values
AMonsterFactory::AMonsterFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMonsterFactory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonsterFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MonsterClass.Num() ==0)	return;
	
	SpawnMonsterTime+=DeltaTime;
	if (SpawnMonsterTime > SpawnDelay)
	{
		SpawnMonsterTime=0.f;
		int32 RandIdx = FMath::RandRange(0, MonsterClass.Num()-1);
		ADGSMonster* SpawnMonster = GetWorld()->SpawnActor<ADGSMonster>(MonsterClass[RandIdx],GetActorLocation(),GetActorRotation());
		SpawnDelay = FMath::RandRange(4.5f, 9.0f);
	}
}

