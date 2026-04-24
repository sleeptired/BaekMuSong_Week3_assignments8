// Fill out your copyright notice in the Description page of Project Settings.


#include "Week3BaseItem.h"
#include "Week3Drone.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AWeek3BaseItem::AWeek3BaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);


	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));//이게 뭐랑 충돌할지 설정해준거다
	Collision->SetupAttachment(Scene);
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AWeek3BaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AWeek3BaseItem::OnItemEndOverlap);


	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);
	//이거 실험해보기
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeek3BaseItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!!")));
		ActivateItem(OtherActor);
	}
}

void AWeek3BaseItem::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AWeek3BaseItem::ActivateItem(AActor* Activator)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!")));
}

FName AWeek3BaseItem::GetItemType() const
{
	return ItemType;
}

void AWeek3BaseItem::DestroyItem()
{
	Destroy();
}

