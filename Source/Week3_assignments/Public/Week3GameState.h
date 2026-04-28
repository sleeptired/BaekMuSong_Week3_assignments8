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
	FTimerHandle HUDUpdateTimerHandle;
	FTimerHandle EventTextTimerHandle;

	// 흐름 제어 함수들
	void StartWave();
	void OnWaveTimeUp();
	void EndWave();
	void UpdateTimeRemaining();


	// 현재 점수를 읽는 함수
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 Get_Score() const;
	// 점수를 추가해주는 함수
	UFUNCTION(BlueprintCallable, Category = "Score")
	void Add_Score(int32 Amount);


	//UFUNCTION(BlueprintCallable, Category = "Level")
	//void OnGameOver(bool bIsCleared);


	UFUNCTION(BlueprintCallable, Category = "Level")
	void EndLevel(); // 웨이브 3 클리어 시 

	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver(); // 드론 사망 시 


	//점수 아이템을 다 먹으면 넘어갈 수 있게 적용 생각중
	//Wave에서 생성된 코인 변수
	//드론(캐릭터)이 먹은 코인 변수

	//스포너에서 SpawnWave 반환형 바꾸는거 고민중

	FString CurrentEventMessage;

	// 이벤트 메시지를 지우는 함수
	void HideEventText();

	// 현재 레벨과 웨이브에 맞는 안내 문구를 반환하는 함수
	FString GetWaveEventMessage(int32 LevelIndex, int32 WaveIndex);

	void UpdateHUD();

};
