// Fill out your copyright notice in the Description page of Project Settings.


#include "Week3Gear.h"

// Sets default values
AWeek3Gear::AWeek3Gear()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	// Static Mesh Component를 생성하고 Scene Component에 Attach
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	RotationSpeed = FRotator(90.0f, 0.0f, 0.0f);
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

