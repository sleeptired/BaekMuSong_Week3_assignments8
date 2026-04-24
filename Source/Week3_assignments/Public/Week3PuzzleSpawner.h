// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Week3PuzzleSpawner.generated.h"

class UBoxComponent;
UCLASS()
class WEEK3_ASSIGNMENTS_API AWeek3PuzzleSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeek3PuzzleSpawner();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner|Components")
	class UBoxComponent* SpawnArea;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Settings")
	TArray<TSubclassOf<AActor>> PlatformClassesToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Settings")
	int32 SpawnCount;


private:
	// 1. 랜덤 스폰 위치를 계산해서 반환하는 함수
	FVector GetRandomSpawnLocation() const;

	// 2. 스폰된 액터의 종류를 파악하고 랜덤 속성을 쏴주는 함수
	void ApplyRandomSettings(AActor* SpawnedActor);

};
