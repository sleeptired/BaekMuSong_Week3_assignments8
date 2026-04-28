// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Week3BaseItem.h"
#include "Week3SlowingItem.generated.h"

/**
 * 
 */
UCLASS()
class WEEK3_ASSIGNMENTS_API AWeek3SlowingItem : public AWeek3BaseItem
{
	GENERATED_BODY()
public:
	AWeek3SlowingItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Debuff")
	float SlowDuration;

	virtual void ActivateItem(AActor* Activator) override;
	
};
