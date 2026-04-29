// Fill out your copyright notice in the Description page of Project Settings.


#include "Week3SlowingItem.h"
#include "Week3Drone.h"

AWeek3SlowingItem::AWeek3SlowingItem()
{
	ItemType = "SlowItem";
	SlowDuration = 5.0f;
}

void AWeek3SlowingItem::ActivateItem(AActor* Activator)
{
	if (AWeek3Drone* Drone = Cast<AWeek3Drone>(Activator))
	{
		// 5초 동안 슬로우 부여 (먹을 때마다 5초씩 중첩됨)
		Drone->ApplySlow(SlowDuration);

		// 화면에 알림 표시
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("디버프: 5초간 이동 속도 50% 감소!"));
		DestroyItem();
	}
}
