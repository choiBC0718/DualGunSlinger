// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/DGSGameMode.h"

#include "DGSSaveGame.h"
#include "Components/TextBlock.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/MainWidget.h"

void ADGSGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ADGSGameMode::InitStartMenu()
{
	if (StartMenuClass)
	{
		StartMenu=CreateWidget<UUserWidget>(GetWorld(),StartMenuClass);
		if (StartMenu)
		{
			StartMenu->AddToViewport();
		}
	}
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
	}
}

void ADGSGameMode::InitMainWidget()
{
	BestScore = LoadScoreData();
	if (MainWidgetClass)
	{
		MainWidget=CreateWidget<UMainWidget>(GetWorld(),MainWidgetClass);
		if (MainWidget)
		{
			MainWidget->AddToViewport();
			UpdateScore();
		}
	}
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
	}
}

void ADGSGameMode::AddScore(int32 Point)
{
	CurrentScore+=Point;
	if (CurrentScore>BestScore)
	{
		BestScore=CurrentScore;
		SaveScoreData(BestScore);
	}
	UpdateScore();
}



void ADGSGameMode::UpdateScore()
{
	if (MainWidget)
	{
		MainWidget->ScoreText->SetText(FText::AsNumber(CurrentScore));
		MainWidget->BestScoreText->SetText(FText::AsNumber(BestScore));
	}
}

void ADGSGameMode::SaveScoreData(int32 SaveValue)
{
	if (!SaveGameClass)
		return;

	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(SaveGameClass);
	UDGSSaveGame* SaveGame = Cast<UDGSSaveGame>(SaveGameObject);
	if (SaveGame)
	{
		SaveGame->ScoreData=SaveValue;
		UGameplayStatics::SaveGameToSlot(SaveGame,TEXT("BestScoreData"),0);
	}
}

int32 ADGSGameMode::LoadScoreData()
{
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("BestScoreData"),0))
		return 0;

	USaveGame* SaveGameObject =UGameplayStatics::LoadGameFromSlot(TEXT("BestScoreData"),0);
	if (SaveGameObject)
	{
		UDGSSaveGame* SaveGame = Cast<UDGSSaveGame>(SaveGameObject);
		if (SaveGame)
		{
			return SaveGame->ScoreData;
		}
	}
	return 0;
}
