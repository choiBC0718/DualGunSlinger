// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DGSMonster.h"

// Sets default values
ADGSMonster::ADGSMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void ADGSMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADGSMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADGSMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

