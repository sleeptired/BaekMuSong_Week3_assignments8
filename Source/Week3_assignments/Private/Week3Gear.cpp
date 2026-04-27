// Fill out your copyright notice in the Description page of Project Settings.


#include "Week3Gear.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeek3Gear::AWeek3Gear()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RotationSpeed = FRotator(360.0f, 0.0f, 0.0f);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	CollisionBox->SetBoxExtent(FVector(100.0f, 100.0f, 20.0f));
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionBox->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void AWeek3Gear::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeek3Gear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateRotation(DeltaTime);
}

void AWeek3Gear::SetGearRotation(FRotator NewRotation)
{
	RotationSpeed = NewRotation;
}

FRotator AWeek3Gear::GetGearRotation()
{
	return RotationSpeed;
}

void AWeek3Gear::UpdateRotation(float DeltaTime)
{
	AddActorLocalRotation(RotationSpeed * DeltaTime);
}

