// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Rabbit.h"
#include "Actors/CarrotProjectile.h"
#include "Interfaces/InteractableInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ARabbit::ARabbit()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Boomstick = CreateDefaultSubobject<USpringArmComponent>(TEXT("Boomstick"));
	Boomstick->SetupAttachment(GetRootComponent());

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(Boomstick);

	// Make sure player posses this pawn
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ARabbit::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	OriginalJumpHeight = GetCharacterMovement()->JumpZVelocity;
	PresentJumpHeight = OriginalJumpHeight;
	MaxJumpHeight = GetCharacterMovement()->JumpZVelocity * JumpMultiplier;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(RabbitContext, 0);
		}
	}

}

void ARabbit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	GetCharacterMovement()->JumpZVelocity = PresentJumpHeight;
	Super::Jump();
	PresentJumpHeight = OriginalJumpHeight;
}

void ARabbit::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.Y);
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

void ARabbit::Attack(const FInputActionValue& Value)
{
	GetWorld()->SpawnActor<ACarrotProjectile>(CarrotProjectile, GetActorLocation() + GetActorForwardVector() * 50.f + FVector(0.f, 0.f, 30.f), GetActorRotation());
}



