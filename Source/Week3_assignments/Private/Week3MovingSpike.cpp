// Fill out your copyright notice in the Description page of Project Settings.


#include "Week3MovingSpike.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


FMovingSpikeSettings::FMovingSpikeSettings()
{
	MoveSpeed = 300.0f;
	MaxRange = 1000.0f;
	MoveDirection = FVector(1.0f, 0.0f, 0.0f);

}

AWeek3MovingSpike::AWeek3MovingSpike()
{
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(110.0f);
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(SceneRoot);

	TrapDamage = 20.0f;
}

void AWeek3MovingSpike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateMovement(DeltaTime);
}

void AWeek3MovingSpike::SetPlatformSettings(const FMovingSpikeSettings& NewSettings)
{
	Settings = NewSettings;

	if (!Settings.MoveDirection.IsNearlyZero())
	{
		Settings.MoveDirection.Normalize();
	}
}

void AWeek3MovingSpike::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();

	if (!Settings.MoveDirection.IsNearlyZero())
	{
		Settings.MoveDirection.Normalize();
	}
}

void AWeek3MovingSpike::UpdateMovement(float DeltaTime)
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

