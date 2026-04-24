// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Week3Gear.generated.h"

UCLASS()
class WEEK3_ASSIGNMENTS_API AWeek3Gear : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeek3Gear();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gear|Components")
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear|Movement")
	FRotator RotationSpeed;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Gear|Actions")
	void UpdateRotation(float DeltaTime);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetGearRotation(FRotator NewRotation);
	FRotator GetGearRotation();

};
