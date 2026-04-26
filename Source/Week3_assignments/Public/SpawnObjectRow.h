// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnObjectRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSpawnObjectRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObjectName;

	// 스폰할 아이템/장애물의 클래스 (블루프린트)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ObectClass;

	// 스폰 확률 (예: 10.0 이면 10의 가중치)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance;
};
