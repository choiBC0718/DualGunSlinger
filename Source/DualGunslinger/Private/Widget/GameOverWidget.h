// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* ScoreText;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* PlayTime;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* RestartBtn;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* QuitBtn;

	UFUNCTION()
	void Restart();

	UFUNCTION()
	void Quit();

	void InitGameOver(int32 Score, FString TimeString);
};
