// Fill out your copyright notice in the Description page of Project Settings.


#include "Week3TrapBase.h"
#include "Components/ShapeComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeek3TrapBase::AWeek3TrapBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TrapDamage = 10.0f; // 기본 데미지 설정
}

void AWeek3TrapBase::BeginPlay()
{
	Super::BeginPlay();

	
	// 모든 형태의 충돌체(Box, Sphere 등)를 싹 다 찾아서 배열에 담습니다.
	TArray<UShapeComponent*> AllCollisions;
	GetComponents<UShapeComponent>(AllCollisions);

	// 찾은 모든 충돌체에 부모의 공통 검문소 함수를 한 번에 연결!
	for (UShapeComponent* Collision : AllCollisions)
	{
		if (Collision)
		{
			Collision->OnComponentBeginOverlap.AddDynamic(this, &AWeek3TrapBase::OnTrapOverlap);
		}
	}
	//근데 이게 부모클래스라서 자식 beginplay가 실행되는거 아닌가? Super::BeginPlay();가 해결해줌
}

void AWeek3TrapBase::OnTrapOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		// 플레이어라면 자식들에게 지시
		OnTrapTriggered(OtherActor);
	}
}

void AWeek3TrapBase::OnTrapTriggered(AActor* Target)
{
	if (Target)
	{
		UGameplayStatics::ApplyDamage(Target, TrapDamage, nullptr, this, UDamageType::StaticClass());
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Trap Overlap"));
	}
}

