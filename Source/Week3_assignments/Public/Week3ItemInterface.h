// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Week3ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeek3ItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WEEK3_ASSIGNMENTS_API IWeek3ItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

public:
	// 플레이어가 이 아이템의 범위에 들어왔을 때 호출
	virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;
	// 플레이어가 이 아이템의 범위를 벗어났을 때 호출
	virtual void OnItemEndOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;
	// 아이템이 사용되었을 때 호출
	virtual void ActivateItem(AActor* Activator) = 0;
	// 이 아이템의 타입을 반환 
	virtual FName GetItemType() const = 0;


	//점수 증가용 아이템
	//체력 힐링 아이템
	//디버프 아이템
};
