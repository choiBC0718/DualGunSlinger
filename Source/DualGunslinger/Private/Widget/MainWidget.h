// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* ScoreText;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* BestScoreText;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* TimeText;

	FTimerHandle TimerHandle;
	
	void Timer();
	float GameStartTime;

	FString PlayTime;
};
