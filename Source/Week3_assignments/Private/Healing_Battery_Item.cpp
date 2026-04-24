// Fill out your copyright notice in the Description page of Project Settings.


#include "Healing_Battery_Item.h"

AHealing_Battery_Item::AHealing_Battery_Item()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealing_Battery_Item::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		// 회복 디버그 메시지
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Player gained %d HP!"), HealAmount));

		DestroyItem();
	}
}
