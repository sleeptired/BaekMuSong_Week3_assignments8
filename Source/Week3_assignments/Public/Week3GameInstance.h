// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Week3GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WEEK3_ASSIGNMENTS_API UWeek3GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UWeek3GameInstance();

	// 전체 누적 점수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;

	// 현재 레벨 인덱스 (1, 2, 3)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);
	
};
