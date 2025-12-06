// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DGSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ADGSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainWidget> MainWidgetClass;
	UPROPERTY()
	class UMainWidget* MainWidget;

	void AddScore(int32 Point);
private:
	int32 CurrentScore=0;
	void UpdateScore();
};
