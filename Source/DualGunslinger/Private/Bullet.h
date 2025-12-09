// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DualGunslinger/DualGunslinger.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();

	UPROPERTY(VisibleAnywhere, Category="Components")
	class USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, Category="Components")
	class UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, Category="Components")
	class UProjectileMovementComponent* MovementComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EGunType BulletType;
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category="VFX")
	class UParticleSystem* ExplosionFX;
	UPROPERTY(EditDefaultsOnly, Category="VFX")
	USoundBase* BulletSound;
private:
	UFUNCTION()
	void OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
