// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Rabbit.h"
#include "Actors/CarrotProjectile.h"
#include "Interfaces/InteractableInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "HUD/RabbitMainMenuWidget.h"
#include "HUD/RabbitMainMenuButtonWidget.h"
#include "HUD/GuideUIComponent.h"
#include "Components/TextBlock.h"
#include "MotionWarpingComponent.h"

ARabbit::ARabbit()
{
	PrimaryActorTick.bCanEverTick = true;

	// Player doesn't rotate along camera rotation
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	// For climbing function
	RabbitMovementComponent = GetCharacterMovement();
	RabbitMovementComponent->MaxFlySpeed = MaxRabbitFlySpeed;
	RabbitMovementComponent->BrakingDecelerationFlying = BrakingDecelerationFlyingSpeed;

	RabbitCapsuleComponent = GetCapsuleComponent();

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Boomstick = CreateDefaultSubobject<USpringArmComponent>(TEXT("Boomstick"));
	Boomstick->SetupAttachment(GetRootComponent());

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(Boomstick);

	GuideUIComponent = CreateDefaultSubobject<UGuideUIComponent>(TEXT("GuideUIComponent"));
	GuideUIComponent->SetupAttachment(GetRootComponent());
	GuideUIComponent->SetVisibility(false);

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
	AddOwnedComponent(MotionWarpingComponent);

	// Make sure player posses this pawn
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ARabbit::ClimbRotationTimelineProgress(float Value)
{
	FRotator NewRotation = FMath::Lerp(StartClimbRotation, EndClimbRotation, Value);
	SetActorRotation(NewRotation);
}

void ARabbit::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(RabbitContext, 0);
		}
	}

	Initialize();
}

void ARabbit::Initialize()
{
	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	OriginalJumpHeight = GetCharacterMovement()->JumpZVelocity;
	PresentJumpHeight = OriginalJumpHeight;
	MaxJumpHeight = GetCharacterMovement()->JumpZVelocity * JumpMultiplier;
	ESCAlreadyPressed = false;

	// Controller & UI
	RabbitController = UGameplayStatics::GetPlayerController(this, 0);
	MainMenuWidgetRef = CreateWidget<URabbitMainMenuWidget>(GetWorld(), MainMenuWidget);
	MainMenuButtonWidgetRef = CreateWidget<URabbitMainMenuButtonWidget>(GetWorld(), MainMenuButtonWidget);

	// Climbing
	if (CurveFloat)
	{
		// Bind Timeline to the function
		TimelineProgress.BindUFunction(this, FName("ClimbRotationTimelineProgress"));
		// Bind Curve to the Timeline
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
		CurveTimeline.SetLooping(false);
	}
}

void ARabbit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Pass DeltaTime into the Curve (for Timeline)
	CurveTimeline.TickTimeline(DeltaTime);
}

void ARabbit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARabbit::Move);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ARabbit::Run);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ARabbit::CancelRun);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARabbit::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ARabbit::ChargeJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ARabbit::Jump);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ARabbit::Interact);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ARabbit::Attack);
		EnhancedInputComponent->BindAction(ESCAction, ETriggerEvent::Completed, this, &ARabbit::ESC);
		EnhancedInputComponent->BindAction(ClimbAction, ETriggerEvent::Completed, this, &ARabbit::Climb);
	}

}

void ARabbit::ChargeJump(const FInputActionValue& Value)
{
	if (PresentJumpHeight < MaxJumpHeight)
	{
		PresentJumpHeight += 5;
	}
}

void ARabbit::Jump()
{
	EMovementMode currentMovementMode = RabbitMovementComponent->MovementMode;
	if (currentMovementMode != EMovementMode::MOVE_Flying)
	{
		GetCharacterMovement()->JumpZVelocity = PresentJumpHeight;
		Super::Jump();
		PresentJumpHeight = OriginalJumpHeight;
	}
	else
	{
		StopClimbing();
	}
}

void ARabbit::Move(const FInputActionValue& Value)
{
	if (isInAnimationMontage) return;

	EMovementMode currentMovementMode = RabbitMovementComponent->MovementMode;
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (currentMovementMode == EMovementMode::MOVE_Walking || currentMovementMode == EMovementMode::MOVE_Falling) // Walking & Jumping
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, MovementVector.X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.Y);
	}
	else if (currentMovementMode == EMovementMode::MOVE_Flying) // Climbing
	{
		const FVector UpDirection = GetActorUpVector();
		Climbing(UpDirection, MovementVector.X);
		const FVector RightDirection = GetActorRightVector();
		Climbing(RightDirection, MovementVector.Y);
	}

}

void ARabbit::Run(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * 2;
}

void ARabbit::CancelRun(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ARabbit::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookVector.Y);
	AddControllerYawInput(LookVector.X);
}

void ARabbit::Interact(const FInputActionValue& Value)
{
	// Remember this
	// Example -> ETraceQueryType MyTraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
	// Example2 -> TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

	TArray<AActor*> IgnoredActors;
	IgnoredActors.AddUnique(this);
	
	FHitResult HitResult;

	// Practice tracing channel more
	bool bHasHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this,
		GetActorLocation(),
		GetActorLocation(),
		100.f,
		ObjectTypesArray,
		false,
		IgnoredActors,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	// Execute interface
	if (bHasHit)
	{
		IInteractableInterface* InteractableInterface = Cast<IInteractableInterface>(HitResult.GetActor());
		ACharacter* Character = Cast<ACharacter>(this);

		if (InteractableInterface)
		{
			InteractableInterface->Execute_IsInteracted(HitResult.GetActor(), Character);
		}
	}
}

// Temporarily disabled
void ARabbit::Attack(const FInputActionValue& Value)
{
	GetWorld()->SpawnActor<ACarrotProjectile>(CarrotProjectile, GetActorLocation() + GetActorForwardVector() * 50.f + FVector(0.f, 0.f, 30.f), GetActorRotation());
}

void ARabbit::EnableUI()
{
	RabbitController->SetInputMode(FInputModeGameAndUI());
	MainMenuWidgetRef->AddToViewport();
	MainMenuButtonWidgetRef->AddToViewport();
	MainMenuButtonWidgetRef->StartResumeText();
	RabbitController->SetShowMouseCursor(true);
}

void ARabbit::DisableUI()
{
	RabbitController->SetInputMode(FInputModeGameOnly());
	MainMenuWidgetRef->RemoveFromParent();
	MainMenuButtonWidgetRef->RemoveFromParent();
	RabbitController->SetShowMouseCursor(false);
}

void ARabbit::ESC(const FInputActionValue& Value)
{
	ESCAlreadyPressed = !ESCAlreadyPressed;

	if (MainMenuWidget && MainMenuButtonWidget)
	{
		if (MainMenuWidgetRef && MainMenuButtonWidgetRef)
		{
			if (ESCAlreadyPressed)
			{
				EnableUI();
			}
			else
			{
				DisableUI();
			}
		}
	}
}

void ARabbit::Climb(const FInputActionValue& Value)
{
	if (RabbitMovementComponent->MovementMode == EMovementMode::MOVE_Flying)
	{
		StopClimbing();
		return;
	}

	// Prep for line tracing
	float TraceDistance = 100.f;
	FVector StartingPoint = GetActorLocation();
	FVector EndingPoint = GetActorForwardVector() * TraceDistance + GetActorLocation();

	FHitResult HitResult = BeginLineTrace(ECollisionChannel::ECC_EngineTraceChannel1, StartingPoint, EndingPoint, FColor::Green);

	if (HitResult.GetActor() != nullptr)
	{
		// Prevent changing rotation while climbing
		RabbitMovementComponent->bOrientRotationToMovement = false;

		// Start climbing
		RabbitMovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);

		// Get offset from the wall by calculating vector from wall and capsule radius
		float CapsuleRadius = RabbitCapsuleComponent->GetScaledCapsuleRadius();
		FVector ImpactVector = HitResult.ImpactNormal;
		FVector WallOffsetVector = ImpactVector * CapsuleRadius;
		FVector WallOffsetLocation = HitResult.Location + WallOffsetVector;

		// Flip vector to face the wall
		FVector FlippedImpactVector = HitResult.ImpactNormal * -1.f;
		FRotator FaceRotation = UKismetMathLibrary::MakeRotFromX(FlippedImpactVector);

		// Attach player to the wall (climbing)
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		UKismetSystemLibrary::MoveComponentTo(RootComponent, WallOffsetLocation, FaceRotation, false, false, 0.2f, true, EMoveComponentAction::Move, LatentInfo);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No Hit Object"));
	}

}

void ARabbit::Climbing(FVector WorldDirection, float ScaleValue)
{
	// Prep for line tracing
	float TraceDistance = 100.f;
	FVector StartingPoint = GetActorLocation();
	FVector EndingPoint = GetActorForwardVector() * TraceDistance + GetActorLocation();

	// Trace for currently climbing wall
	FHitResult HitResult = BeginLineTrace(ECollisionChannel::ECC_EngineTraceChannel1, StartingPoint, EndingPoint, FColor::Green);

	AddMovementInput(WorldDirection, ScaleValue);
	FVector VectorToWall = HitResult.ImpactNormal * -1.f;
	FRotator RotatorToWall = UKismetMathLibrary::MakeRotFromX(VectorToWall);

	// Rotation for Timeline lerp animation
	StartClimbRotation = GetActorRotation();
	EndClimbRotation = RotatorToWall;
	CurveTimeline.PlayFromStart();

	if (HitResult.GetActor() != nullptr)
	{
		// Detecting ledge to mount
		CalculateLedgeToMount(TraceDistance);
		// **Detecting ledge to mount
	}
	else
	{
		StopClimbing();
	}
}

void ARabbit::CalculateLedgeToMount(float TraceDistance)
{
	FVector HeadStartingPoint = GetActorLocation() + FVector(0.f, 0.f, 100.f);
	FVector HeadEndingPoint = GetActorForwardVector() * TraceDistance + HeadStartingPoint;

	FHitResult LedgeHitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bLedgeHasHit = GetWorld()->LineTraceSingleByChannel(
		LedgeHitResult,
		HeadStartingPoint,
		HeadEndingPoint,
		ECollisionChannel::ECC_EngineTraceChannel1,
		Params
	);

	// Debug line for Ledge
	UKismetSystemLibrary::DrawDebugLine(
		this,
		HeadStartingPoint,
		HeadEndingPoint,
		FColor::Cyan,
		5.0f,
		1.f
	);

	if (!bLedgeHasHit)
	{
		int maxIndex = 20;
		int step = 3;
		float HeightToCheckFrom = 90.f;

		for (int index = 0; index <= maxIndex; index++)
		{
			int stepLength = index * step;
			FVector LedgeCheckStartLocation = (GetActorForwardVector() * stepLength) + (LedgeHitResult.TraceStart + FVector(0.f, 0.f, HeightToCheckFrom));
			FVector LedgeCheckEndLocation = LedgeCheckStartLocation - FVector(0.f, 0.f, HeightToCheckFrom + 10);

			FHitResult LedgeTopHitResult;

			bool bLedgeTopHasHit = GetWorld()->LineTraceSingleByChannel(
				LedgeTopHitResult,
				LedgeCheckStartLocation,
				LedgeCheckEndLocation,
				ECollisionChannel::ECC_EngineTraceChannel1,
				Params
			);

			// Debug line for LedgeTop
			UKismetSystemLibrary::DrawDebugLine(
				this,
				LedgeCheckStartLocation,
				LedgeCheckEndLocation,
				FColor::Purple,
				5.0f,
				1.f
			);

			if (bLedgeTopHasHit)
			{
				UKismetSystemLibrary::DrawDebugSphere(
					this,
					LedgeTopHitResult.Location,
					20.f,
					12,
					FColor::Purple,
					5.f
				);

				FTimerHandle AnimationTimerHandler;

				// 1st motion warping (Climbing ledge vertically)
				FVector TopLedgeVerticalWarpingLocation = FVector(GetActorLocation().X, GetActorLocation().Y, LedgeTopHitResult.Location.Z);
				MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(FName("LedgeClimbUp"), TopLedgeVerticalWarpingLocation, GetActorRotation());

				// 2nd motion warping (Climbing ledge horizontally)
				FVector TopLedgeHorizontalWarpingLocation = FVector(LedgeTopHitResult.Location.X, LedgeTopHitResult.Location.Y, GetActorLocation().Z + 100.f);
				MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(FName("LedgeClimbForward"), TopLedgeHorizontalWarpingLocation, GetActorRotation());

				// Disable player input and enter animation
				isInAnimationMontage = true;
				PlayAnimMontage(MountLedgeMontage);

				// Stop climbing after animation finished
				GetWorldTimerManager().SetTimer(AnimationTimerHandler, this, &ARabbit::StopClimbing, MountLedgeMontage->GetPlayLength(), false);

				// break out of for loop (for checking ledge top)
				break;
			}
		}
	}
}

void ARabbit::StopClimbing()
{
	// Enable orient rotation and movement mode back after that we fixi player rotation
	RabbitMovementComponent->bOrientRotationToMovement = true;
	RabbitMovementComponent->SetMovementMode(EMovementMode::MOVE_Falling);
	SetActorRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f));
	
	// Enable input back after finishing certain animation
	if (isInAnimationMontage)
	{
		isInAnimationMontage = false;
	}
}

FHitResult ARabbit::BeginLineTrace(ECollisionChannel TraceChannel, FVector StartingPoint, FVector EndingPoint, FColor DebugColor)
{
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHasHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartingPoint,
		EndingPoint,
		TraceChannel,
		Params
	);

	UKismetSystemLibrary::DrawDebugLine(
		this,
		StartingPoint,
		EndingPoint,
		DebugColor,
		5.0f,
		1.f
	);

	return HitResult;
}

// Q: What is the difference between Line Trace By Channel and Line Trace For Objects?
// A: Channels are used for things like visibility and the camera, and almost exclusively have to do with tracing. 
// Object Types are the physics types of Actors with collision in your scene, such as Pawns, Vehicles, Destructible Actors, and so on.
// Line trace by channel only checks the channel setting of the objects, which gives it a cheaper cost as its probably buffered somewhere for indexing purposes.
// Line trace by objects will trace all objects and return the associated type requested.