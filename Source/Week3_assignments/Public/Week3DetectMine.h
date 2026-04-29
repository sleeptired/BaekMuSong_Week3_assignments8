	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Week3TrapBase.h"
#include "Week3DetectMine.generated.h"


USTRUCT(BlueprintType)
struct FMineTrapSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplosionDelay; // 기본 폭발 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplosionRadius; // 기본 폭발 범위

	bool bIsTriggered;

	FMineTrapSettings();
};


class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class WEEK3_ASSIGNMENTS_API AWeek3DetectMine : public AWeek3TrapBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeek3DetectMine();
	void SetTrapSettings(const FMineTrapSettings& NewSettings);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnTrapTriggered(AActor* Target) override;

	UPROPERTY(VisibleAnywhere, Category = "Mine|Components")
	class USphereComponent* TriggerSphere; // 감지 및 폭발 범위

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mine|Component")
	UStaticMeshComponent* RangeIndicator;

	// 2. 지뢰 설정값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine|Settings")
	FMineTrapSettings Settings;

	FTimerHandle ExplosionTimerHandle;

	void Explode();

};
