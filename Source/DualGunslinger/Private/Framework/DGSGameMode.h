// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DGSGameMode.generated.h"

class UDGSSaveGame;
class USaveGame;
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
	TSubclassOf<class UUserWidget> StartMenuClass;
	UPROPERTY()
	class UUserWidget* StartMenu;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainWidget> MainWidgetClass;
	UPROPERTY()
	class UMainWidget* MainWidget;

	UFUNCTION(BlueprintCallable)
	void InitStartMenu();
	UFUNCTION(BlueprintCallable)
	void InitMainWidget();
	
	void AddScore(int32 Point);
	int32 GetScore() const {return CurrentScore;}

	void SaveScoreData(int32 SaveValue);
	int32 LoadScoreData();
	
private:
	int32 CurrentScore=0;
	int32 BestScore=0;
	
	void UpdateScore();

	UPROPERTY(EditDefaultsOnly, Category = "SaveSystem")
	TSubclassOf<UDGSSaveGame> SaveGameClass;
};
