// Fill out your copyright notice in the Description page of Project Settings.

#include "Week3MovePlatform.h"

AWeek3MovePlatform::AWeek3MovePlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

}

void AWeek3MovePlatform::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();

	if (!Settings.MoveDirection.IsNearlyZero())
	{
		Settings.MoveDirection.Normalize();
	}
}

void AWeek3MovePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateMovement(DeltaTime);
}

void AWeek3MovePlatform::SetPlatformSettings(const FMovePlatformSettings& NewSettings)
{
	// 구조체째로 덮어쓰기
	Settings = NewSettings;

	if (!Settings.MoveDirection.IsNearlyZero())
	{
		Settings.MoveDirection.Normalize();
	}
}

void AWeek3MovePlatform::UpdateMovement(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();

	// 방향 * 속도 * 시간
	FVector DeltaLocation = Settings.MoveDirection * Settings.MoveSpeed * DeltaTime;

	AddActorWorldOffset(DeltaLocation, true);

	float DistanceMoved = FVector::Distance(StartLocation, CurrentLocation);

	if (DistanceMoved >= Settings.MaxRange)
	{
		float Overshoot = DistanceMoved - Settings.MaxRange;
		FVector Correction = -Settings.MoveDirection * Overshoot;

		AddActorWorldOffset(Correction, true);

		// 방향 반전
		Settings.MoveDirection *= -1.0f;

		// 왕복 기준점 갱신
		StartLocation = GetActorLocation();
	}
}

FMovePlatformSettings::FMovePlatformSettings()
{
	MoveSpeed = 300.0f;
	MaxRange = 1000.0f;
	MoveDirection = FVector(1.0f, 0.0f, 0.0f);
}
