// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Week3BaseItem.h"
#include "Week3ReverseControlItem.generated.h"

/**
 * 
 */
UCLASS()
class WEEK3_ASSIGNMENTS_API AWeek3ReverseControlItem : public AWeek3BaseItem
{
	GENERATED_BODY()
public:
	AWeek3ReverseControlItem();

	// 블루프린트에서 조작 반전 유지 시간을 조절할 수 있도록 노출
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Debuff")
	float ReverseDuration;

	virtual void ActivateItem(AActor* Activator) override;
};
