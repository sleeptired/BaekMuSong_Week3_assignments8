// Fill out your copyright notice in the Description page of Project Settings.


#include "Week3DroneController.h"
#include "Week3GameState.h"
#include "Week3GameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

AWeek3DroneController::AWeek3DroneController():
	DroneMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr)
{
}

UUserWidget* AWeek3DroneController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void AWeek3DroneController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}

		AWeek3GameState* Week3GameState = GetWorld() ? GetWorld()->GetGameState<AWeek3GameState>() : nullptr;
		if (Week3GameState)
		{
			Week3GameState->UpdateHUD();
		}
	}
}

void AWeek3DroneController::ShowMainMenu(bool bIsRestart, bool bIsTimeOver = false)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());

			UpdateMainMenuUI(bIsRestart, bIsTimeOver);
		}
		//(구)게임버튼 텍스트 정하는곳
		//if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		//{
		//	if (bIsRestart)
		//	{
		//		//GameOver로직
		//		ButtonText->SetText(FText::FromString("Restart"));
		//
		//	}
		//	else
		//	{
		//
		//		ButtonText->SetText(FText::FromString("Start"));
		//	}
		//}
		//
		////게임오버 텍스트 애니메이션
		//if(bIsRestart)
		//{
		//	//게임오버 텍스트 애니메이션
		//	UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
		//	if (PlayAnimFunc)
		//	{
		//		MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
		//	}
		//	//Total스코어 텍스트
		//	if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TotalScore")))
		//	{
		//		if (UWeek3GameInstance* Week3GameInstance = Cast<UWeek3GameInstance>(UGameplayStatics::GetGameInstance(this)))
		//		{
		//			TotalScoreText->SetText(FText::FromString(
		//				FString::Printf(TEXT("Total Score: %d"), Week3GameInstance->TotalScore)
		//			));
		//		}
		//	}
		//}
		//else//게임제목 이런 텍스트 나오는 곳
		//{
		//
		//}
	}
}

void AWeek3DroneController::StartGame()
{	

	if (UWeek3GameInstance* Week3GameInstance = Cast<UWeek3GameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		Week3GameInstance->TotalScore = 0;
		Week3GameInstance->CurrentLevelIndex = 1;
	}
	SetPause(false);
	UGameplayStatics::OpenLevel(GetWorld(), FName("Level_1"));

}

void AWeek3DroneController::OnExitButtonClicked()
{
	//메인메뉴에서 클릭했는지, 게임중에 클릭했는지
	if (GetWorld()->GetMapName().Contains("MainMenu"))
	{
		UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, true);
	}
	else
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu_Level"));
	}
}


void AWeek3DroneController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) 
		{
			if (DroneMappingContext)
			{
				Subsystem->AddMappingContext(DroneMappingContext, 0);
			}
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MainMenu_Level"))
	{
		ShowMainMenu(false);
	}

	//삭제
	//if (HUDWidgetClass)
	//{
	//	HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
	//	if (HUDWidgetInstance)
	//	{
	//		HUDWidgetInstance->AddToViewport();
	//	}
	//}
	//


	//삭제
	//AWeek3GameState* Week3GameState = GetWorld() ? GetWorld()->GetGameState<AWeek3GameState>() : nullptr;
	//if (Week3GameState)
	//{
	//	Week3GameState->UpdateHUD();
	//}
	//
}

void AWeek3DroneController::UpdateMainMenuUI(bool bIsRestart, bool bIsTimeOver)
{
	if (!MainMenuWidgetInstance) return;

	UTextBlock* TitleText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("GameNameText")));
	UTextBlock* ResultText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("GameOverText")));
	UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("TotalScore")));
	UTextBlock* StartButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText")));
	UTextBlock* ExitButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("ExitButtonText")));

	if (!bIsRestart) // 초기 메뉴
	{
		if (TitleText) TitleText->SetVisibility(ESlateVisibility::Visible);
		if (ResultText) ResultText->SetVisibility(ESlateVisibility::Hidden);
		if (TotalScoreText) TotalScoreText->SetVisibility(ESlateVisibility::Hidden);
		if (StartButtonText) StartButtonText->SetText(FText::FromString(TEXT("Start")));
		if (ExitButtonText) ExitButtonText->SetText(FText::FromString(TEXT("Exit")));
	}
	else // 결과 화면
	{
		if (TitleText) TitleText->SetVisibility(ESlateVisibility::Hidden);
		if (ResultText) ResultText->SetVisibility(ESlateVisibility::Visible);
		if (TotalScoreText) TotalScoreText->SetVisibility(ESlateVisibility::Visible);
		if (StartButtonText) StartButtonText->SetText(FText::FromString(TEXT("Restart")));
		if (ExitButtonText) ExitButtonText->SetText(FText::FromString(TEXT("Main Menu")));

		if (ResultText)//게임 시간으로 끝났는지, 장애물때문에 죽었는지 텍스트 정리 
		{
			ResultText->SetText(bIsTimeOver ? FText::FromString(TEXT("Time Over!")) : FText::FromString(TEXT("Game Over!")));
		}

		UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
		if (PlayAnimFunc)
		{
			MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
		}

		if (TotalScoreText)
		{
			if (UWeek3GameInstance* GI = Cast<UWeek3GameInstance>(UGameplayStatics::GetGameInstance(this)))
			{
				TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total Score: %d"), GI->TotalScore)));
			}
		}
	}
}
