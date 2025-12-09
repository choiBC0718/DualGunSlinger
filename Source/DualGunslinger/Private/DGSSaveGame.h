// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DGSSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class UDGSSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 ScoreData;
};
