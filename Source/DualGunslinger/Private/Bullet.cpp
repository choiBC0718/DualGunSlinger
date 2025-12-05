// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Components/SphereComponent.h"

// Sets default values
ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComp=CreateDefaultSubobject<USphereComponent>("Collision Comp");
	SetRootComponent(SphereComp);
	SphereComp->SetSphereRadius(20.f);

	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
	MeshComp->SetupAttachment(SphereComp);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

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
}

