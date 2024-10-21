// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "Archer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

//void UPlayerAnimInstance::NativeInitializeAnimation()
//{
//	Super::NativeInitializeAnimation();
//
//
//}

UPlayerAnimInstance::UPlayerAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("/Script/Engine.AnimMontage'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Animations/Primary_Fire_Med_Montage.Primary_Fire_Med_Montage'"));

	if (AM.Succeeded())
	{
		FireMontage = AM.Object;
	}
}

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	auto Pawn = TryGetPawnOwner();

	if (IsValid(Pawn))
	{
		Archer = Cast<AArcher>(Pawn);
		if (IsValid(Archer))
		{
			CharacterMovement = Archer->GetCharacterMovement();
		}
	}

}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(Archer))
	{
		Velocity = CharacterMovement->Velocity;
		FRotator Rotation = Archer->GetActorRotation();
		FVector UnrotateVector = Rotation.UnrotateVector(Velocity);

		UnrotateVector.Normalize();

		Speed = Velocity.Size2D();

		auto Acceleration = CharacterMovement->GetCurrentAcceleration();
		ShouldMove = Speed > 3.f && Acceleration != FVector::Zero();

		IsFalling = CharacterMovement->IsFalling();

		AimRotation = Archer->GetBaseAimRotation();
		FRotator RotFromX = UKismetMathLibrary::MakeRotFromX(Velocity);

		FRotator DeltaRotation = AimRotation - RotFromX;
		DeltaRotation.Normalize();  

		YawOffset = DeltaRotation.Yaw;

	}

}

void UPlayerAnimInstance::PlayFireMontage()
{
	if (IsValid(FireMontage))
	{
		if (!Montage_IsPlaying(FireMontage))
		{
			Montage_Play(FireMontage);
		}
	}
}
