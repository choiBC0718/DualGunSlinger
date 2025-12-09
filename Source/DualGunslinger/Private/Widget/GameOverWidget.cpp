// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/GameOverWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RestartBtn->OnClicked.AddDynamic(this, &UGameOverWidget::Restart);
	QuitBtn->OnClicked.AddDynamic(this, &UGameOverWidget::Quit);
}

void UGameOverWidget::Restart()
{
	UGameplayStatics::OpenLevel(GetWorld(),"DualGunslinger");
}

void UGameOverWidget::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(),EQuitPreference::Quit,false);
}

void UGameOverWidget::InitGameOver(int32 Score, FString TimeString)
{
	if (ScoreText)
		ScoreText->SetText(FText::AsNumber(Score));
	if (PlayTime)
		PlayTime->SetText(FText::FromString(TimeString));
}
