// Fill out your copyright notice in the Description page of Project Settings.


#include "Week3GameState.h"
#include "Week3GameInstance.h"
#include "Week3PuzzleSpawner.h"
#include "Week3DroneController.h"
#include "Kismet/GameplayStatics.h"

AWeek3GameState::AWeek3GameState()
{
	Score = 0;
	CurrentWave = 1;
	MaxWaves = 3; // 3웨이브
	WaveDuration = 60.0f; // 30초
	WaveTimeRemaining = 0;
}

void AWeek3GameState::BeginPlay()
{
	Super::BeginPlay();

	FString CurrentMapName = GetWorld()->GetMapName();
	if (!CurrentMapName.Contains("MenuLevel"))
	{
		StartWave();
	}
}

void AWeek3GameState::StartWave()
{
	WaveDuration = WaveDuration - (CurrentWave - 1) * 15.0f;

	// 혹시 모를 추가 웨이브를 위해 최소 시간(예: 15초) 아래로는 안 내려가게 방어 코드를 넣으면 좋습니다.
	if (WaveDuration < 15.0f)
	{
		WaveDuration = 15.0f;
	}

	WaveTimeRemaining = (int32)WaveDuration;

	// 스포너에게 스폰 명령 (웨이브 증가에 따른 개수 증가는 스포너가 알아서 처리함)
	TArray<AActor*> FoundSpawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeek3PuzzleSpawner::StaticClass(), FoundSpawners);
	if (FoundSpawners.Num() > 0)
	{
		if (AWeek3PuzzleSpawner* Spawner = Cast<AWeek3PuzzleSpawner>(FoundSpawners[0]))
		{
			Spawner->SpawnWave(CurrentWave);
		}
	}

	GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AWeek3GameState::OnWaveTimeUp, WaveDuration, false);
	GetWorldTimerManager().SetTimer(UICountdownTimerHandle, this, &AWeek3GameState::UpdateTimeRemaining, 1.0f, true);

	UE_LOG(LogTemp, Warning, TEXT("Wave %d 시작!"), CurrentWave);
}

void AWeek3GameState::OnWaveTimeUp()
{
	EndWave();
}

void AWeek3GameState::EndWave()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	GetWorldTimerManager().ClearTimer(UICountdownTimerHandle);

	// 스포너 청소 지시
	TArray<AActor*> FoundSpawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeek3PuzzleSpawner::StaticClass(), FoundSpawners);
	if (FoundSpawners.Num() > 0)
	{
		if (AWeek3PuzzleSpawner* Spawner = Cast<AWeek3PuzzleSpawner>(FoundSpawners[0]))
		{
			Spawner->ClearWave_Obejct();
		}
	}

	CurrentWave++;

	if (CurrentWave > MaxWaves)
	{
		OnGameOver(true); // 레벨 클리어
	}
	else
	{
		StartWave(); // 다음 웨이브 진행
	}
}

void AWeek3GameState::UpdateTimeRemaining()
{
	//UI강의 로직보기

	WaveTimeRemaining--;
}

void AWeek3GameState::OnGameOver(bool bIsCleared)
{
	//GameInstance확인
	if (bIsCleared)
	{
		if (UWeek3GameInstance* GI = Cast<UWeek3GameInstance>(GetGameInstance()))
		{
			GI->AddToScore(Score);
			GI->CurrentLevelIndex++;

			if (GI->CurrentLevelIndex > 3) // 3레벨까지 다 깼다면
			{
				if (AWeek3DroneController* PC = Cast<AWeek3DroneController>(GetWorld()->GetFirstPlayerController()))
				{
					//PC->ShowGameOver(); // 승리 화면용 게임오버 창 띄우기
				}
			}
			else
			{
				// 다음 맵 열기
				FString NextLevelName = FString::Printf(TEXT("Level_%d"), GI->CurrentLevelIndex);
				UGameplayStatics::OpenLevel(GetWorld(), FName(*NextLevelName));
			}
		}
	}
	else
	{
		// 사망 시: 메뉴 UI 띄우기
		if (AWeek3DroneController* PC = Cast<AWeek3DroneController>(GetWorld()->GetFirstPlayerController()))
		{
			//PC->ShowGameOver();
		}
	}
}

void AWeek3GameState::Add_Score(int32 Amount)
{
	if (UGameInstance* GameInsatance = GetGameInstance())
	{
		UWeek3GameInstance* Week3GameInstance = Cast<UWeek3GameInstance>(GameInsatance);
		if (Week3GameInstance)
		{
			Week3GameInstance->AddToScore(Amount);
		}
	}
	//Score += Amount;
	//UE_LOG(LogTemp, Warning, TEXT("Scroe: %d"), Score);
}

int32 AWeek3GameState::Get_Score() const
{
	return Score;
}
