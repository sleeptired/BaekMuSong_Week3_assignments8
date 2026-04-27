// Fill out your copyright notice in the Description page of Project Settings.


#include "Week3PuzzleSpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Week3MovePlatform.h" //장애물 변경
#include "Week3Gear.h"
#include "SpawnObjectRow.h"

// Sets default values
AWeek3PuzzleSpawner::AWeek3PuzzleSpawner()
{

	PrimaryActorTick.bCanEverTick = false;


	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SetRootComponent(SpawnArea);
	SpawnArea->SetBoxExtent(FVector(1000.0f, 1000.0f, 500.0f)); 
	SpawnArea->SetCollisionProfileName(TEXT("NoCollision"));   

	ObjectCount = 20; // 기본 스폰 개수


}

// Called when the game starts or when spawned
void AWeek3PuzzleSpawner::BeginPlay()
{
	Super::BeginPlay();
}

FVector AWeek3PuzzleSpawner::GetRandomSpawnLocation() const
{
	const FVector BoxExtent = SpawnArea->GetScaledBoxExtent();
	const FVector BoxOrigin = SpawnArea->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

void AWeek3PuzzleSpawner::ApplyRandomSettings(AActor* SpawnedActor)
{
	//장애물 변경
	if (AWeek3MovePlatform* MovingPlatform = Cast<AWeek3MovePlatform>(SpawnedActor))
	{
		FMovePlatformSettings RandomSettings;
		RandomSettings.MoveSpeed = FMath::RandRange(100.0f, 800.0f);
		RandomSettings.MaxRange = FMath::RandRange(500.0f, 1500.0f);
		RandomSettings.MoveDirection = FVector(
			FMath::RandRange(-1.0f, 1.0f),
			FMath::RandRange(-1.0f, 1.0f),
			FMath::RandRange(-1.0f, 1.0f)
		);
		MovingPlatform->SetPlatformSettings(RandomSettings);
	}
	else if (AWeek3Gear* Gear = Cast<AWeek3Gear>(SpawnedActor))
	{
		//Gear 피치값(첫번째 매개변수)만 수정하기
		FRotator RandomSpin = FRotator(
			FMath::RandRange(-180.0f, 180.0f),
			FMath::RandRange(-180.0f, 180.0f),
			FMath::RandRange(-180.0f, 180.0f)
		);
		Gear->SetGearRotation(RandomSpin);
	}
}

FSpawnObjectRow* AWeek3PuzzleSpawner::GetRandomItemFromTable(UDataTable* DataTable) const
{
	if (!DataTable) return nullptr;

	TArray<FSpawnObjectRow*> AllRows;
	static const FString ContextString(TEXT("SpawnContext"));//혹시나 데이터 테이블에 오류가 생길시 나올 구분하는 stirng
	DataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty())
	{
		return nullptr;
	}

	// 1. 전체 확률(가중치)의 합 계산
	float TotalChance = 0.0f;
	for (const FSpawnObjectRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}

	// 0 ~ TotalChance 사이의 랜덤 값 뽑기
	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;

	// 누적 확률로 어느 아이템인지 확인
	for (FSpawnObjectRow* Row : AllRows)
	{
		if (Row)
		{
			AccumulateChance += Row->SpawnChance;
			if (RandValue <= AccumulateChance)
			{
				return Row;
			}
		}
	}

	return nullptr;
}

void AWeek3PuzzleSpawner::SpawnWave(int32 WaveIndex)
{
	int32 ArrayIndex = WaveIndex - 1;

	if (!WaveDataTables.IsValidIndex(ArrayIndex) || WaveDataTables[ArrayIndex] == nullptr)
	{
		//데이터 테이블이 설정안되어있으면 
		return;
	}

	UDataTable* CurrentTable = WaveDataTables[ArrayIndex];

	//Wave따라 아이템 나오는 갯수 증가
	int32 CurrentWaveSpawnCount = ObjectCount + (ArrayIndex * 10);

	for (int32 i = 0; i < CurrentWaveSpawnCount; i++)
	{
		if (FSpawnObjectRow* SelectedRow = GetRandomItemFromTable(CurrentTable))
		{
			if (UClass* ActualClass = SelectedRow->ObectClass.Get())
			{
				FVector RandomLocation = GetRandomSpawnLocation();
				FRotator RandomRotation = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);

				AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActualClass, RandomLocation, RandomRotation);

				if (SpawnedActor)
				{
					ApplyRandomSettings(SpawnedActor);
					SpawnedItems.Add(SpawnedActor);
				}
			}
		}
	}
}

void AWeek3PuzzleSpawner::ClearWave_Obejct()
{
	// 모든 아이템 파괴
	for (AActor* Item : SpawnedItems)
	{
		// 아이템이 유효하고, 파괴 대기 상태가 아니라면
		if (IsValid(Item))
		{
			Item->Destroy();
		}
	}
	// 명부 싹 비우기
	SpawnedItems.Empty();
}

