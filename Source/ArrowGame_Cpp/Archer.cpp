// Fill out your copyright notice in the Description page of Project Settings.


#include "Archer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerAnimInstance.h"

// Sets default values
AArcher::AArcher()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));
	SpringArm->bUsePawnControlRotation = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Sparrow.Sparrow'"));
	if (SM.Succeeded())
	{
		//UE_LOG(LogTemp, Log, TEXT("TEST"));
		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	}
}

// Called when the game starts or when spawned
void AArcher::BeginPlay()
{
	Super::BeginPlay();

	auto AnimInstance = GetMesh()->GetAnimInstance();
	PlayerAnimInstance = Cast<UPlayerAnimInstance>(AnimInstance);
	
}

// Called every frame
void AArcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AArcher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AArcher::KeyUpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AArcher::KeyLeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUpDown"), this, &AArcher::MouseLookUpDown);
	PlayerInputComponent->BindAxis(TEXT("LookLeftRight"), this, &AArcher::MouseLookLeftRight);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AArcher::Jump);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AArcher::Fire);

}

void AArcher::KeyUpDown(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AArcher::KeyLeftRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void AArcher::MouseLookLeftRight(float Value)
{
	AddControllerYawInput(Value);
}

void AArcher::MouseLookUpDown(float Value)
{
	AddControllerPitchInput(Value);
}

void AArcher::Fire()
{
	if (IsValid(PlayerAnimInstance))
	{
		PlayerAnimInstance->PlayFireMontage();

	}
	
}

