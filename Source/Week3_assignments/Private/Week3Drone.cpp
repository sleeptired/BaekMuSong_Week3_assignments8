// Fill out your copyright notice in the Description page of Project Settings.


#include "Week3Drone.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Week3DroneController.h"
#include "Components/TextBlock.h"
#include "Week3GameState.h"
#include "DrawDebugHelpers.h"

// Sets default values
AWeek3Drone::AWeek3Drone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
	SphereComp->SetSimulatePhysics(false); //off

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(RootComponent);
	SkeletalMeshComp->SetSimulatePhysics(false); //off

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);
	StaticMeshComp->SetSimulatePhysics(false);//off

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 400.0f;
	SpringArmComp->bUsePawnControlRotation = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName); 
	CameraComp->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(StaticMeshComp);
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	MoveInput = FVector(0, 0, 0);
	LookInput = FVector(0, 0, 0);

	MoveSpeed = 1000.0f;
	RotationSpeed = 100.0f;
	bIsGrounded = false;

	SphereComp->SetCollisionProfileName(TEXT("Pawn"));//충돌처리 예방

	Gravity = -980.0f;
	FallSpeed = 0.0f;

	ShiftSpeed = 1000.0f;
	UpSpeed = 2500.0f;

	BaseMeshRotation = StaticMeshComp->GetRelativeRotation();

	//체력 설정
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	//무적판정
	bIsInvincible = false;

	//디버프 판정
	bIsSlowed = false;
	bIsReversed = false;
}

float AWeek3Drone::GetHealth() const
{
	return CurrentHealth;
}

void AWeek3Drone::AddHealth(float Amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);
	UpdateOverheadHP();
	UE_LOG(LogTemp, Warning, TEXT("Health increased to: %f"), CurrentHealth);
}

//void AWeek3Drone::ApplySlow(float Duration)
//{
//	bIsSlowed = true;
//
//	float RemainingTime = GetWorldTimerManager().GetTimerRemaining(SlowTimerHandle);
//	RemainingTime = (RemainingTime > 0.0f) ? RemainingTime + Duration : Duration;
//
//	GetWorldTimerManager().SetTimer(SlowTimerHandle, this, &AWeek3Drone::ClearSlow, RemainingTime, false);
//}
//
//void AWeek3Drone::ClearSlow()
//{
//	bIsSlowed = false;
//}

// Called when the game starts or when spawned
void AWeek3Drone::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP();
}


void AWeek3Drone::OnDeath()
{
	AWeek3GameState* Week3GameState = GetWorld() ? GetWorld()->GetGameState<AWeek3GameState>() : nullptr;
	if (Week3GameState)
	{
		Week3GameState->OnGameOver();
	}
}

void AWeek3Drone::UpdateOverheadHP()
{
	if (!OverheadWidget)
	{
		return;
	}

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance)
	{
		return;
	}

	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHealth, MaxHealth)));
	}
}

float AWeek3Drone::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	if (bIsInvincible)
	{
		return 0.0f;
	}

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); //실제 데미지를 들고온다.

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

	UpdateOverheadHP();

	UE_LOG(LogTemp, Warning, TEXT("Health Decreased to: %f"), CurrentHealth);

	if (CurrentHealth <= 0.0f)
	{
		OnDeath();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("무적 시간 시작"));
		bIsInvincible = true;
		GetWorld()->GetTimerManager().SetTimer(InvincibilityTimerHandle, this, &AWeek3Drone::DisableInvincibility, 2.0f, false);
	}
	return 0.0f;
}

void AWeek3Drone::DisableInvincibility()
{
	bIsInvincible = false;
	UE_LOG(LogTemp, Log, TEXT("무적 시간 끝"));
}

// Called every frame
void AWeek3Drone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Accumulator += DeltaTime;

	while (Accumulator >= TimeStep) 
	{
		CustomTick(TimeStep);

		Accumulator -= TimeStep;
	}

}


void AWeek3Drone::CustomTick(float FixedDeltaTime)
{

	UpdateRotation(FixedDeltaTime);               // 1. 회전 업데이트
	UpdateGroundDetection();                      // 2. 지면 감지
	UpdateGravityAndHovering(FixedDeltaTime);     // 3. 중력 및 호버링
	UpdateMovement(FixedDeltaTime);               // 4. 전후좌우 이동 업데이트
	//UpdateBanking(FixedDeltaTime); 모션 업데이트 나중에 추가예정

	LookInput.X = 0.0f;
	LookInput.Y = 0.0f;
}

// Called to bind functionality to input
void AWeek3Drone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		if (AWeek3DroneController* PlayerController = Cast<AWeek3DroneController>(GetController()))
		{
			if (PlayerController->MoveAction) 
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AWeek3Drone::Move
				);

				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Completed,
					this,
					&AWeek3Drone::Move
				);
			}

			if (PlayerController->LookAction) 
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AWeek3Drone::Look
				);

				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Completed,
					this,
					&AWeek3Drone::Look
				);
			}
		}
	}
}

void AWeek3Drone::Move(const FInputActionValue& Value)
{
	MoveInput = Value.Get<FVector>();
}

void AWeek3Drone::Look(const FInputActionValue& Value)
{
	LookInput = Value.Get<FVector>();
}


void AWeek3Drone::UpdateRotation(float DeltaTime)
{
	if (!LookInput.IsNearlyZero())
	{
		FRotator NewRotation = FRotator(
			LookInput.Y * RotationSpeed * DeltaTime,
			LookInput.X * RotationSpeed * DeltaTime,
			LookInput.Z * RotationSpeed * DeltaTime
		);
		AddActorLocalRotation(NewRotation, true);
	}
}

void AWeek3Drone::UpdateGroundDetection()
{
	FVector StartLocation = GetActorLocation();
	float SphereRadius = SphereComp->GetScaledSphereRadius();
	FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, -(SphereRadius + 5.0f));

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	bIsGrounded = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

	// 디버그 드로잉
	FColor LineColor = bIsGrounded ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, LineColor, false, -1.0f, 0, 2.0f);

	if (bIsGrounded)
	{
		DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Yellow, false, -1.0f);
	}
}

void AWeek3Drone::UpdateGravityAndHovering(float DeltaTime)
{
	float ZInput = MoveInput.Z;

	if (bIsGrounded && ZInput <= 0.0f)
	{
		FallSpeed = 0.0f; // 착지 순간 낙하 속도 초기화
	}
	else
	{
		FallSpeed += Gravity * DeltaTime;

		if (ZInput > 0.0f)
		{
			FallSpeed += UpSpeed * DeltaTime; // 상승
		}
		else if (ZInput < 0.0f)
		{
			FallSpeed -= ShiftSpeed * DeltaTime; // 하강
		}
		else
		{
			// 호버링 유지
			FallSpeed += (-Gravity) * DeltaTime;
			float TargetHoverSpeed = -20.0f;
			FallSpeed = FMath::FInterpTo(FallSpeed, TargetHoverSpeed, DeltaTime, 3.0f);
		}
	}

	FVector GravityMove = FVector(0.0f, 0.0f, FallSpeed * DeltaTime);
	AddActorWorldOffset(GravityMove, true);
}

void AWeek3Drone::UpdateMovement(float DeltaTime)
{
	FVector LocalInput = MoveInput;
	LocalInput.Z = 0.0f; // 이동 시 상하 입력 무시

	if (!LocalInput.IsNearlyZero())
	{
		if (bIsGrounded)
		{
			// 지상 이동 (파고들기 방지)
			FVector WorldDir = GetActorRotation().RotateVector(LocalInput);
			WorldDir.Z = 0.0f;

			if (!WorldDir.IsNearlyZero())
			{
				WorldDir.Normalize();
				FVector DeltaLocation = WorldDir * MoveSpeed * DeltaTime;
				AddActorWorldOffset(DeltaLocation, true);
			}
		}
		else
		{
			// 공중 에어 컨트롤 (속도 40%)
			float AirSpeed = MoveSpeed * 0.4f;
			FVector DeltaLocation = LocalInput * AirSpeed * DeltaTime;
			AddActorLocalOffset(DeltaLocation, true);
		}
	}
}
