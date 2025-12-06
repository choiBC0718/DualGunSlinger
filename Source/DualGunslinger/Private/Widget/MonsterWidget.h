// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterWidget.generated.h"

/**
 * 
 */
UCLASS()
class UMonsterWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadOnly,Category="Setup")
	class ADGSMonster* OwnerMonster;

	UFUNCTION(BlueprintImplementableEvent)
	void Init();
};
