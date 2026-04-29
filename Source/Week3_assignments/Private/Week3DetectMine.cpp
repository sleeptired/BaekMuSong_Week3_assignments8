// Fill out your copyright notice in the Description page of Project Settings.


#include "Week3DetectMine.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
AWeek3DetectMine::AWeek3DetectMine()
{
	// 1. 단일 충돌체 설정
	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionSphere"));
	TriggerSphere->SetupAttachment(SceneRoot);
	TriggerSphere->InitSphereRadius(Settings.ExplosionRadius);
	TriggerSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// 2. 장판 인디케이터 설정
	RangeIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RangeIndicator"));
	RangeIndicator->SetupAttachment(SceneRoot);
	RangeIndicator->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 장판은 충돌하면 안 됨
	RangeIndicator->SetVisibility(false);
}

void AWeek3DetectMine::SetTrapSettings(const FMineTrapSettings& NewSettings)
{
	Settings = NewSettings;

	if (TriggerSphere)
	{
		TriggerSphere->SetSphereRadius(Settings.ExplosionRadius);
	}
}

// Called when the game starts or when spawned
void AWeek3DetectMine::BeginPlay()
{
	Super::BeginPlay();

	if (RangeIndicator)
	{
		float Scale = Settings.ExplosionRadius / 50.0f;
		RangeIndicator->SetWorldScale3D(FVector(Scale, Scale, Scale)); 
	}
}

void AWeek3DetectMine::OnTrapTriggered(AActor* Target)
{
	if (!Settings.bIsTriggered)
	{
		Settings.bIsTriggered = true;
		//UE_LOG(LogTemp, Warning, TEXT("지뢰 작동! %f초 뒤 폭발합니다!"), Settings.ExplosionDelay);

		// 빨간색 장판 켜기
		if (RangeIndicator)
		{
			//UE_LOG(LogTemp, Warning, TEXT("범위 작동 테스트"));
			RangeIndicator->SetVisibility(true);
		}

		// 타이머 시작
		GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AWeek3DetectMine::Explode, Settings.ExplosionDelay, false);
	}
}


void AWeek3DetectMine::Explode()
{
	// 폭발 범위 안의 '모든' 플레이어에게 데미지
	TArray<AActor*> OverlappingActors;
	TriggerSphere->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			// 부모 클래스에 있는 TrapDamage 변수를 활용
			UGameplayStatics::ApplyDamage(Actor, TrapDamage, nullptr, this, UDamageType::StaticClass());
		}
	}

	//UE_LOG(LogTemp, Error, TEXT("쾅! 지뢰 폭발 완료!"));

	// 파티클이나 사운드를 넣고 싶다면 이 위치에서 재생하시면 됩니다.

	Destroy(); // 지뢰 파괴
}


FMineTrapSettings::FMineTrapSettings()
{
	ExplosionDelay = 3.0f;
	ExplosionRadius = 600.0f;
	bIsTriggered = false;
}
