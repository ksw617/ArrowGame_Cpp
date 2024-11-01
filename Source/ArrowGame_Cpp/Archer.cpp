// Fill out your copyright notice in the Description page of Project Settings.


#include "Archer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "PlayerAnimInstance.h"
#include "Arrow.h"
#include "MyActorComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"

//UI
#include "Components/WidgetComponent.h"
#include "MyUserWidget.h"

// Sets default values
AArcher::AArcher()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);


	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SocketOffset = FVector(0.f, 120.f, 75.f); // Ãß°¡

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Sparrow.Sparrow'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	}


	MyActorComponent = CreateDefaultSubobject<UMyActorComponent>(TEXT("MyActorComponent"));

	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UMyUserWidget> UW(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_HpBar.WBP_HpBar_C'"));
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
		HpBar->SetDrawSize(FVector2D(200.f, 25.f));
		HpBar->SetRelativeLocation(FVector(0.f, 0.f, 200.f));

	}

}


float AArcher::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	MyActorComponent->OnDamaged(Damage);
	return Damage;
}

// Called when the game starts or when spawned
void AArcher::BeginPlay()
{
	Super::BeginPlay();

	auto AnimInstance = GetMesh()->GetAnimInstance();
	PlayerAnimInstance = Cast<UPlayerAnimInstance>(AnimInstance);

	auto HpWidget = Cast<UMyUserWidget>(HpBar->GetUserWidgetObject());
	if (HpWidget)
	{			
		HpWidget->BindHp(MyActorComponent);

	}


}

// Called to bind functionality to input
void AArcher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Jumping
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AArcher::Fire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AArcher::StopFire);


		//// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AArcher::Move);
		//
		//// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AArcher::Look);
	}


}


void AArcher::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);

}

void AArcher::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

	}
}

void AArcher::Fire()
{
	if (IsValid(PlayerAnimInstance))
	{
		PlayerAnimInstance->PlayFireMontage();

		float AttackRange = 1000.f;

		FHitResult HitResult;

		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);

		FVector AimLocation = CameraManager->GetCameraLocation();
		FVector TargetLocation = AimLocation + CameraManager->GetActorForwardVector() * AttackRange;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		bool Result = GetWorld()->LineTraceSingleByChannel
		(
			OUT HitResult,
			AimLocation,
			TargetLocation,
			ECollisionChannel::ECC_GameTraceChannel1,
			Params
		);

		if (Result)
		{
			TargetLocation = HitResult.ImpactPoint;
			DrawDebugLine(GetWorld(), AimLocation, TargetLocation, FColor::Green, true);

		}
		else
		{
			DrawDebugLine(GetWorld(), AimLocation, TargetLocation, FColor::Red, true);
		}

		FTransform SocketTransform = GetMesh()->GetSocketTransform(FName("ArrowSocket"));
		FVector SocketLocation = SocketTransform.GetLocation();
		
		FVector DeltaVector = TargetLocation - SocketLocation;

		FRotator SocketRotation = FRotationMatrix::MakeFromX(DeltaVector).Rotator();
		FActorSpawnParameters ArrowParams;
		ArrowParams.Owner = this;
		ArrowParams.Instigator = this;
		auto MyArrow = GetWorld()->SpawnActor<AArrow>(SocketLocation, SocketRotation, ArrowParams);

	}

}

void AArcher::StopFire()
{
	UE_LOG(LogTemp, Log, TEXT("StopFire"));
}


