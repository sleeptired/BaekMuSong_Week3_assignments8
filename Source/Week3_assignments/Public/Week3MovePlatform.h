// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Week3MovePlatform.generated.h"


USTRUCT(BlueprintType)
struct FMovePlatformSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MoveDirection;

	//구조체 생성자로 초기화를 시킴
	FMovePlatformSettings();
};

UCLASS()
class WEEK3_ASSIGNMENTS_API AWeek3MovePlatform : public AActor
{
	GENERATED_BODY()

public:
	AWeek3MovePlatform();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform|Components")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Platform|Components")
	UStaticMeshComponent* StaticMeshComp;

	// 💡 구조체 변수 단 하나로 통일!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Movement")
	FMovePlatformSettings Settings;

	FVector StartLocation;

	virtual void BeginPlay() override;
	void UpdateMovement(float DeltaTime);

public:
	virtual void Tick(float DeltaTime) override;

	void SetPlatformSettings(const FMovePlatformSettings& NewSettings);
};
