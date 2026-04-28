// Fill out your copyright notice in the Description page of Project Settings.


#include "Week3GameState.h"
#include "Week3GameInstance.h"
#include "Week3PuzzleSpawner.h"
#include "Week3DroneController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

AWeek3GameState::AWeek3GameState()
{
	Score = 0;
	CurrentWave = 1;
	MaxWaves = 3; // 3웨이브
	WaveDuration = 10.0f; 
	WaveTimeRemaining = 0;
}

void AWeek3GameState::BeginPlay()
{
	Super::BeginPlay();

	//UI 테스트용
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AWeek3GameState::UpdateHUD,
		0.1f,
		true
	);
	//이거 활용해서 메인 메뉴로 가게 하는거 생각하기
	FString CurrentMapName = GetWorld()->GetMapName();
	if (!CurrentMapName.Contains("MenuLevel"))
	{
		StartWave();
	}
}

void AWeek3GameState::StartWave()
{
	//게임 시작했으니 메뉴 UI 삭제
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AWeek3DroneController* Week3DroneController = Cast<AWeek3DroneController>(PlayerController))
		{
			Week3DroneController->ShowGameHUD();
		}
	}
	//테스트용이라서 나중에 주석풀기
	//float BaseDuration = 60.0f;
	//WaveDuration = BaseDuration - (CurrentWave - 1) * 15.0f;
	//
	//if (WaveDuration < 15.0f)
	//{
	//	WaveDuration = 15.0f;
	//}

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
		EndLevel(); // 레벨 클리어
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

//void AWeek3GameState::OnGameOver(bool bIsCleared)
//{
//	//여기 로직 수정하기
//	
//	//GameInstance확인
//	if (bIsCleared)
//	{
//		if (UWeek3GameInstance* GI = Cast<UWeek3GameInstance>(GetGameInstance()))
//		{
//			GI->AddToScore(Score);
//			GI->CurrentLevelIndex++;
//
//			if (GI->CurrentLevelIndex > 3) // 3레벨까지 다 깼다면
//			{
//				if (AWeek3DroneController* PC = Cast<AWeek3DroneController>(GetWorld()->GetFirstPlayerController()))
//				{
//					//일단 테스트용(게임 종료부분)
//					if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
//					{
//						if (AWeek3DroneController* Week3DroneController = Cast<AWeek3DroneController>(PlayerController))
//						{
//							Week3DroneController->ShowMainMenu(true);
//						}
//					}
//					//
//				}
//			}
//			else
//			{
//				// 다음 맵 열기
//				FString NextLevelName = FString::Printf(TEXT("Level_%d"), GI->CurrentLevelIndex);
//				UGameplayStatics::OpenLevel(GetWorld(), FName(*NextLevelName));
//			}
//		}
//	}
//	else
//	{
//		// 사망 시: 메뉴 UI 띄우기
//		if (AWeek3DroneController* PC = Cast<AWeek3DroneController>(GetWorld()->GetFirstPlayerController()))
//		{
//			//일단 테스트용(게임 종료부분)
//			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
//			{
//				if (AWeek3DroneController* Week3DroneController = Cast<AWeek3DroneController>(PlayerController))
//				{
//					Week3DroneController->ShowMainMenu(true);
//				}
//			}
//			//
//		}
//	}
//}

void AWeek3GameState::EndLevel()
{
	if (UWeek3GameInstance* GI = Cast<UWeek3GameInstance>(GetGameInstance()))
	{
		GI->AddToScore(Score);
		GI->CurrentLevelIndex++;

		if (GI->CurrentLevelIndex > 3) // 3레벨까지 다 깼다면
		{
			if (AWeek3DroneController* Week3DroneController = Cast<AWeek3DroneController>(GetWorld()->GetFirstPlayerController()))
			{
				Week3DroneController->SetPause(true);
				Week3DroneController->ShowMainMenu(true,true); //TimeOver
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

void AWeek3GameState::OnGameOver()
{
	UE_LOG(LogTemp, Error, TEXT("Game Over! 플레이어가 파괴되었습니다."));

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AWeek3DroneController* Week3DroneController = Cast<AWeek3DroneController>(PlayerController))
		{
			Week3DroneController->SetPause(true);     // 게임 멈춤
			Week3DroneController->ShowMainMenu(true, false); // GameOver
		}
	}
}

void AWeek3GameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if(AWeek3DroneController* Week3DroneController = Cast<AWeek3DroneController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = Week3DroneController->GetHUDWidget())
			{
				//Wave다 시간
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}
				//Score 누적 (레벨넘어가도)
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UWeek3GameInstance* Week3GameInstance = Cast<UWeek3GameInstance>(GameInstance);
						if (Week3GameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Week3GameInstance->TotalScore)));
						}
					}
				}
				//Level UI업데이트
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UWeek3GameInstance* Week3GameInstance = Cast<UWeek3GameInstance>(GameInstance);
						if (Week3GameInstance)
						{
							LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), Week3GameInstance->CurrentLevelIndex)));
						}
					}
				}
				//Wave추가
				if (UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
				{
					WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), CurrentWave)));
				}
			}
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
