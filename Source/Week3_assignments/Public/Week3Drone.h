// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Week3Drone.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
struct FInputActionValue;

UCLASS()
class WEEK3_ASSIGNMENTS_API AWeek3Drone : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWeek3Drone();


	// 현재 체력을 가져오는 함수
	UFUNCTION(BlueprintPure, Category = "HP")
	float GetHealth() const;
	// 체력을 회복시키는 함수
	UFUNCTION(BlueprintCallable, Category = "HP")
	void AddHealth(float Amount);

	// 디버프 아이템용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debuff")
	bool bIsSlowed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debuff")
	bool bIsReversed;

	// 디버프 해제를 위한 타이머 
	FTimerHandle SlowTimerHandle;
	FTimerHandle ReverseTimerHandle;

	// 아이템에서 디버프를 걸 때 부르는 함수
	//void ApplySlow(float Duration);
	//void ClearSlow();
	//
	//void ApplyReverse(float Duration);
	//void ClearReverse();
	//
	//// UI 남은 시간을 알려주는 함수
	//UFUNCTION(BlueprintPure, Category = "Debuff")
	//float GetSlowTimeRemaining() const;
	//
	//UFUNCTION(BlueprintPure, Category = "Debuff")
	//float GetReverseTimeRemaining() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Comp
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	//3D위젯
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;
	 

	FVector MoveInput;
	FVector LookInput;


	UPROPERTY(EditAnywhere, Category = "DroneSettings")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, Category = "DroneSettings")
	float RotationSpeed;


	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	float Accumulator = 0.0f;

	const float TimeStep = 1.0f / 60.0f; //60fps

	void CustomTick(float FixedDeltaTime);//Custom Tick

	bool bIsGrounded;

	UFUNCTION()
	void UpdateRotation(float DeltaTime);

	UFUNCTION()
	void UpdateGroundDetection();

	UFUNCTION()
	void UpdateGravityAndHovering(float DeltaTime);

	UFUNCTION()
	void UpdateMovement(float DeltaTime);

	//UFUNCTION()
	//void UpdateBanking(float DeltaTime); 나중에 추가예정

	UPROPERTY(EditAnywhere, Category = "DroneSettings")
	float Gravity; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DroneSettings|Debug")
	float FallSpeed;

	float ShiftSpeed;

	float UpSpeed;

	FRotator BaseMeshRotation;

	//체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HP")
	float CurrentHealth;

	// 사망 처리 함수 (체력이 0 이하가 되었을 때 호출)
	void OnDeath();

	void UpdateOverheadHP();

	// 데미지 처리 함수 - 외부로부터 데미지를 받을 때 호출됨
    // 또는 AActor의 TakeDamage()를 오버라이드
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	// 무적 상태인지 확인하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	bool bIsInvincible;

	// 무적 시간 타이머
	FTimerHandle InvincibilityTimerHandle;

	// 무적 상태를 풀어주는 함수
	void DisableInvincibility();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
