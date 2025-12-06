// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/DGSGameMode.h"

#include "Components/TextBlock.h"
#include "Widget/MainWidget.h"

void ADGSGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (MainWidgetClass!=nullptr)
	{
		MainWidget=CreateWidget<UMainWidget>(GetWorld(),MainWidgetClass);
		if (MainWidget!=nullptr)
		{
			MainWidget->AddToViewport();
		}
	}
}

void ADGSGameMode::AddScore(int32 Point)
{
	CurrentScore+=Point;
	UpdateScore();
}

void ADGSGameMode::UpdateScore()
{
	if (MainWidget)
	{
		MainWidget->ScoreText->SetText(FText::AsNumber(CurrentScore));
	}
}
