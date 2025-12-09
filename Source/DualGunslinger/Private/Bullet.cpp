// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Character/DGSMonster.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComp=CreateDefaultSubobject<USphereComponent>("Collision Comp");
	SetRootComponent(SphereComp);
	SphereComp->SetCollisionProfileName("Bullet");
	SphereComp->SetSphereRadius(20.f);
	SphereComp->SetGenerateOverlapEvents(true);

	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
	MeshComp->SetupAttachment(SphereComp);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MovementComp=CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlap);
	SphereComp->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADGSMonster* Monster = Cast<ADGSMonster>(OtherActor);
	if (Monster)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX,GetActorLocation(),GetActorRotation());
		UGameplayStatics::PlaySound2D(GetWorld(),BulletSound);
		Monster->MonsterHit(BulletType);
	}
	Destroy();
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

