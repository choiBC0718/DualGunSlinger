// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Die
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UMonsterFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMonsterFSM();

	void IdleState();
	void MoveState();
	void AttackState();
	void DestroyOnLevel();
	void DieState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyState mState;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnDamageProcess();
	
private:
	UPROPERTY(EditDefaultsOnly, Category="FSM")
	float IdleDelayTime =2.f;
	UPROPERTY(EditDefaultsOnly, Category="FSM")
	float AttackRange = 150.f;
	UPROPERTY(EditDefaultsOnly, Category="FSM")
	float AttackDelayTime =2.f;
	UPROPERTY(EditDefaultsOnly, Category="FSM")
	float MoveSpeedRate =0.1f;
	UPROPERTY(EditDefaultsOnly, Category="FSM")
	float DieHeight = -50.f;
	UPROPERTY(EditDefaultsOnly, Category="FSM")
	float DieSpeed = 5.f;
	
	float CurrentTime=0.f;

	UPROPERTY()
	class ADGSPlayer* Target;
	UPROPERTY()
	class ADGSMonster* Me;

	FTimerHandle DestroyTimerHandle;
};
