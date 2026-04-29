// Fill out your copyright notice in the Description page of Project Settings.


#include "Week3GameInstance.h"

UWeek3GameInstance::UWeek3GameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 1;
}

void UWeek3GameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	//UE_LOG(LogTemp, Warning, TEXT("총 누적 점수 갱신: %d"), TotalScore);
}
