// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Week3BaseItem.h"
#include "Score_Battery_Item.generated.h"

/**
 * 
 */
UCLASS()
class WEEK3_ASSIGNMENTS_API AScore_Battery_Item : public AWeek3BaseItem
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;

	AScore_Battery_Item();

	virtual void ActivateItem(AActor* Activator) override;
};
