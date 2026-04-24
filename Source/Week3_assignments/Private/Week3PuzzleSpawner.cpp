// Fill out your copyright notice in the Description page of Project Settings.


#include "Week3PuzzleSpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Week3MovePlatform.h" 
#include "Week3Gear.h"         

// Sets default values
AWeek3PuzzleSpawner::AWeek3PuzzleSpawner()
{

	PrimaryActorTick.bCanEverTick = false;


	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SetRootComponent(SpawnArea);
	SpawnArea->SetBoxExtent(FVector(1000.0f, 1000.0f, 500.0f)); 
	SpawnArea->SetCollisionProfileName(TEXT("NoCollision"));   

	SpawnCount = 10; // 기본 스폰 개수

}

// Called when the game starts or when spawned
void AWeek3PuzzleSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (PlatformClassesToSpawn.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("No Object to Spawn"));
		return;
	}

	const FVector BoxExtent = SpawnArea->GetScaledBoxExtent();
	const FVector BoxOrigin = SpawnArea->GetComponentLocation();


	for (int32 i = 0; i < SpawnCount; i++)
	{
		FVector RandomLocation = GetRandomSpawnLocation();
		FRotator RandomRotation = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);

		int32 RandomIndex = FMath::RandRange(0, PlatformClassesToSpawn.Num() - 1);
		TSubclassOf<AActor> SelectedClass = PlatformClassesToSpawn[RandomIndex];

		if (!SelectedClass)
		{
			continue;
		}

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(SelectedClass, RandomLocation, RandomRotation);

		if (SpawnedActor)
		{
			ApplyRandomSettings(SpawnedActor);
		}
	}
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
		FRotator RandomSpin = FRotator(
			FMath::RandRange(-180.0f, 180.0f),
			FMath::RandRange(-180.0f, 180.0f),
			FMath::RandRange(-180.0f, 180.0f)
		);
		Gear->SetGearRotation(RandomSpin);
	}
}

// Called every frame
void AWeek3PuzzleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

