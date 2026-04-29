// Fill out your copyright notice in the Description page of Project Settings.


#include "Week3ReverseControlItem.h"
#include "Week3Drone.h"

AWeek3ReverseControlItem::AWeek3ReverseControlItem()
{
	ItemType = "SlowItem";
	ReverseDuration = 5.0f;
}

void AWeek3ReverseControlItem::ActivateItem(AActor* Activator)
{
	if (AWeek3Drone* Drone = Cast<AWeek3Drone>(Activator))
	{
		Drone->ApplyReverse(ReverseDuration);

		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("디버프: %f초간 컨트롤 반전"), ReverseDuration));
		DestroyItem();
	}
}
