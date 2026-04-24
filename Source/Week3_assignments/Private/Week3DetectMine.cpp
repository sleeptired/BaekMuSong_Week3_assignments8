// Fill out your copyright notice in the Description page of Project Settings.


#include "Week3DetectMine.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AWeek3DetectMine::AWeek3DetectMine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	MineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MineMesh"));
	MineMesh->SetupAttachment(SceneRoot);

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->SetupAttachment(SceneRoot);

	bIsTriggered = false;

	// 생성 시 기본 크기 세팅
	TriggerSphere->InitSphereRadius(Settings.ExplosionRadius);
	TriggerSphere->SetCollisionProfileName(TEXT("Trigger"));

}

void AWeek3DetectMine::SetTrapSettings(const FMineTrapSettings& NewSettings)
{
	Settings = NewSettings;

	// 스포너가 던져준 랜덤 범위(Radius)에 맞춰서, 
	// 실제 눈에 안 보이는 파란색 충돌 구체의 크기도 같이 갱신해 줍니다!
	if (TriggerSphere)
	{
		TriggerSphere->SetSphereRadius(Settings.ExplosionRadius);
	}
}

// Called when the game starts or when spawned
void AWeek3DetectMine::BeginPlay()
{
	Super::BeginPlay();

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeek3DetectMine::OnOverlapBegin);
}

void AWeek3DetectMine::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsTriggered && OtherActor && OtherActor != this)
	{
		bIsTriggered = true;

		// Settings 안에 있는 값을 꺼내 씁니다.
		UE_LOG(LogTemp, Warning, TEXT("[지뢰 작동] 범위 내 침입 감지! %f초 뒤 폭발!"), Settings.ExplosionDelay);

		GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AWeek3DetectMine::Explode, Settings.ExplosionDelay, false);
	}
}

void AWeek3DetectMine::Explode()
{
	UE_LOG(LogTemp, Error, TEXT("지뢰 폭발 완료!"));


	Destroy();
}

// Called every frame
void AWeek3DetectMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FMineTrapSettings::FMineTrapSettings()
{
	ExplosionDelay = 3.0f;
	ExplosionRadius = 300.0f;
}
