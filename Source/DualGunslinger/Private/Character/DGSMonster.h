// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DualGunslinger/DualGunslinger.h"
#include "GameFramework/Character.h"
#include "DGSMonster.generated.h"

class ADGSGameMode;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnMonsterHPChanged, int32, CurPistol, int32, CurRifle, int32, MaxPistol,
                                              int32, MaxRifle);

UCLASS()
class ADGSMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADGSMonster();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FSMComponent")
	class UMonsterFSM* FSM;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UWidgetComponent* HPBarWidget;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMonsterHPChanged OnHPChanged;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Monster Stat")
	EMonsterType MonsterType = EMonsterType::PistolWeak;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster Stat")
	int32 MaxPistolHP = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster Stat")
	int32 MaxRifleHP = 3;

	int32 CurPistolHP;
	int32 CurRifleHP;

	UPROPERTY(BlueprintReadOnly)
	bool bIsDead=false;

	void MonsterHit(EGunType GunType);
private:
	UFUNCTION()
	void MonsterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category="VFX")
	class UParticleSystem* ExplosionFX;

	UPROPERTY()
	TObjectPtr<ADGSGameMode> GameMode;
};
