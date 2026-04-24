// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Week3BaseItem.h"
#include "Healing_Battery_Item.generated.h"

/**
 * 
 */
UCLASS()
class WEEK3_ASSIGNMENTS_API AHealing_Battery_Item : public AWeek3BaseItem
{
	GENERATED_BODY()
public:
	AHealing_Battery_Item();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float HealAmount;

	virtual void ActivateItem(AActor* Activator) override;
};
