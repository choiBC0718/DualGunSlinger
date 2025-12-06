// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterFactory.generated.h"

UCLASS()
class AMonsterFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Spawn")
	float SpawnDelay = 3.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Spawn")
	TArray<TSubclassOf<class ADGSMonster>> MonsterClass;

private:
	float SpawnMonsterTime=0.f;
};
