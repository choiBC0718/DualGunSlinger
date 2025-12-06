// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "DGSPlayer.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerHPChanged, int32, CurrentHp, int32, MaxHp);
UCLASS()
class ADGSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADGSPlayer();

	UPROPERTY(VisibleAnywhere, Category="View")
	class USpringArmComponent* SpringArmComp;
	
	UPROPERTY(VisibleAnywhere, Category="View")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category="GunMesh")
	class USkeletalMeshComponent* PistolMesh;
	UPROPERTY(VisibleAnywhere, Category="GunMesh")
	class USkeletalMeshComponent* RifleMesh;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerHPChanged OnHPChanged;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
private:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_ChangeGun;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_Move;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_Shoot;
	
	void ChangeGun();
	void Shoot(const FInputActionValue& InputActionValue);
	void Move(const FInputActionValue& InputActionValue);
	
	FVector MoveDirection;
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Player Stat",BlueprintReadOnly)
	int32 MaxHealth = 6;
	UPROPERTY(VisibleAnywhere, Category = "Player Stat", BlueprintReadOnly)
	int32 CurrentHp;
	UPROPERTY(EditDefaultsOnly, Category="Player Stat",BlueprintReadOnly)
	float MoveSpeed = 500.f;
	UPROPERTY(EditDefaultsOnly, Category="Player Stat",BlueprintReadOnly)
	float ShootRatio = 1.f;
	UPROPERTY(EditDefaultsOnly, Category="Player Stat",BlueprintReadOnly)
	float BulletSpeed = 320.f;
	UPROPERTY(EditDefaultsOnly, Category="Player Stat",BlueprintReadOnly)
	float BulletMaxDist = 1000.f;

	float ShootTime=0.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Bullet")
	TSubclassOf<class ABullet> PistolBulletClass;

	UPROPERTY(EditDefaultsOnly, Category="Bullet")
	TSubclassOf<class ABullet> RifleBulletClass;

	UFUNCTION()
	void PlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UPROPERTY(BlueprintReadOnly)
	bool bIsRifleMode = true;

	void GetDamaged();
};
