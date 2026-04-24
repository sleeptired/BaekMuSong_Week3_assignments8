// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Week3GameState.generated.h"

/**
 * 
 */
UCLASS()
class WEEK3_ASSIGNMENTS_API AWeek3GameState : public AGameState
{
	GENERATED_BODY()
public:
	AWeek3GameState();

	virtual void BeginPlay() override;

	//UI 바인딩용 변수들
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentWave;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 MaxWaves;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	float WaveDuration; // 웨이브 유지 시간

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 WaveTimeRemaining; // UI 표시용 남은 시간

	FTimerHandle WaveTimerHandle;
	FTimerHandle UICountdownTimerHandle; // 1초마다 남은 시간 깎는 용도

	// 흐름 제어 함수들
	void StartWave();
	void OnWaveTimeUp();
	void EndWave();
	void UpdateTimeRemaining();

	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver(bool bIsCleared);
};
