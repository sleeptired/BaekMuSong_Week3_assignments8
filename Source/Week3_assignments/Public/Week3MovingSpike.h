// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Week3TrapBase.h"
#include "Week3MovingSpike.generated.h"

/**
 * 
 */

class USphereComponent;
USTRUCT(BlueprintType)
struct FMovingSpikeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MoveDirection;

	//구조체 생성자로 초기화를 시킴
	FMovingSpikeSettings();
};

UCLASS()
class WEEK3_ASSIGNMENTS_API AWeek3MovingSpike : public AWeek3TrapBase
{
	GENERATED_BODY()
public:
	AWeek3MovingSpike();

	virtual void Tick(float DeltaTime) override;

	void SetPlatformSettings(const FMovingSpikeSettings& NewSettings);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Movement")
	FMovingSpikeSettings Settings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform|Component")
	USphereComponent* Collision;

	FVector StartLocation;

	virtual void BeginPlay() override;

	void UpdateMovement(float DeltaTime);

};
