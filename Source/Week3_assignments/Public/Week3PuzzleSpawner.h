// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnObjectRow.h"
#include "Week3PuzzleSpawner.generated.h"

class UBoxComponent;
class UDataTable;
UCLASS()
class WEEK3_ASSIGNMENTS_API AWeek3PuzzleSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeek3PuzzleSpawner();

	void SpawnWave(int32 WaveIndex);

	void ClearWave_Obejct();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner|Components")
	class UBoxComponent* SpawnArea;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<UDataTable*> WaveDataTables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 ObjectCount;


private:
	UPROPERTY()
	TArray<AActor*> SpawnedItems;

	// 랜덤 스폰 위치를 계산해서 반환하는 함수
	FVector GetRandomSpawnLocation() const;

	// 스폰된 액터의 종류를 파악하고 랜덤 속성을 쏴주는 함수
	void ApplyRandomSettings(AActor* SpawnedActor);

	// 특정 데이터 테이블에서 확률에 맞춰 아이템을 뽑아오는 함수
	FSpawnObjectRow* GetRandomItemFromTable(UDataTable* DataTable) const;



};
