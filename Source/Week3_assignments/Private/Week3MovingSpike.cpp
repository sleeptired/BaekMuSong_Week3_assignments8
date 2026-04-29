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
	//현재위치
	FVector CurrentLocation = GetActorLocation();

	// 방향 * 속도 * 시간
	FVector DeltaLocation = Settings.MoveDirection * Settings.MoveSpeed * DeltaTime;

	//계산한 DeltaLocation 움직임
	AddActorWorldOffset(DeltaLocation, true);

	//이동한 누적 거리 검사 (출발했던 위치와 지금 위치 사이의 직선거리를 계산
	float DistanceMoved = FVector::Distance(StartLocation, CurrentLocation);

	//최종 목적지에 도달했는지 검사
	if (DistanceMoved >= Settings.MaxRange)
	{
		//초과 이동량 보정(프레임 때문에 MaxRange에 멈추지 못하고 넘어가 버리는 경우를 대비해 오차를 구함)
		float Overshoot = DistanceMoved - Settings.MaxRange;

		// 넘어간 거리만큼 반대 방향으로 되돌려보낼 벡터 생성
		FVector Correction = -Settings.MoveDirection * Overshoot;

		//그만큼 다시 보냄
		AddActorWorldOffset(Correction, true);

		// 방향 반전
		Settings.MoveDirection *= -1.0f;

		// 왕복 기준점 갱신 (다시 출발해야해서 현재위치를 시작지점으로 설정)
		StartLocation = GetActorLocation();
	}
}

