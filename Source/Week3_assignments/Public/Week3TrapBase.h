// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Week3TrapBase.generated.h"

class UShapeComponent;

UCLASS()
class WEEK3_ASSIGNMENTS_API AWeek3TrapBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeek3TrapBase();

protected:
	virtual void BeginPlay() override;

	// 공통 뼈대 (자식들이 가져다 씁니다)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trap|Component")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trap|Component")
	UStaticMeshComponent* StaticMeshComp;

	// 공통 속성 (데미지 수치)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Settings")
	float TrapDamage;

	// 자식들이 자기 입맛대로 덮어쓸 수 있는 가상 함수!
	UFUNCTION()
	virtual void OnTrapOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	//필요하면 OnTrapOverlap도 구현

	virtual void OnTrapTriggered(AActor* Target);


};
