// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MainWidget.h"

#include "Components/TextBlock.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (TimeText)
		TimeText->SetText(FText::FromString("00 : 00 : 00"));

	GameStartTime = GetWorld()->GetTimeSeconds();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&UMainWidget::Timer,0.01f,true);
}

void UMainWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}


void UMainWidget::Timer()
{
	if (!TimeText)
		return;

	float CurrentTime = GetWorld()->GetTimeSeconds() - GameStartTime;
	
	int32 Min = (int32)(CurrentTime / 60.f);
	int32 Sec = (int32)(CurrentTime) % 60;
	int32 MSec = (int32)((CurrentTime - (int32)CurrentTime)*100);

	PlayTime = FString::Printf(TEXT("%02d : %02d : %02d"), Min, Sec, MSec);

	TimeText->SetText(FText::FromString(PlayTime));
}
