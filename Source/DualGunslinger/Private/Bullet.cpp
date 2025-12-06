// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Character/DGSMonster.h"
#include "Components/SphereComponent.h"

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
}

void ABullet::TravelMaxDist()
{
	Destroy();
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	float Time = MaxDistance / MoveSpeed;
	GetWorld()->GetTimerManager().SetTimer(BulletTimerHandle,this, &ABullet::TravelMaxDist,Time);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlap);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(GetActorLocation() + GetActorForwardVector()*MoveSpeed*DeltaTime);
}

void ABullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADGSMonster* Monster = Cast<ADGSMonster>(OtherActor);
	if (Monster)
	{
		Monster->MonsterHit(BulletType);
		Destroy();
	}
}

