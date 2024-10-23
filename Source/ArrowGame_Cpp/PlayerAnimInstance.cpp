// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "Archer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


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

		if (ShouldMove || IsFalling)
		{
			RotateYaw = FMath::FInterpTo(RotateYaw, 0.f, DeltaSeconds, 20.f);
			MovingRotation = Archer->GetActorRotation();
			PrevRotation = MovingRotation;
		}
		else
		{
			PrevRotation = MovingRotation;
			MovingRotation = Archer->GetActorRotation();
			FRotator Deleta = MovingRotation - PrevRotation;
			Deleta.Normalize();
			RotateYaw -= Deleta.Yaw;

			float TurnValue = GetCurveValue("Turn");

			if (TurnValue > 0.f)
			{
				PrevDistanceCurve = DistanceCurve;
				DistanceCurve = GetCurveValue("DistanceCurve");
				DeltaDistanceCurve = DistanceCurve - PrevDistanceCurve;
				if (RotateYaw > 0.f)
				{
					RotateYaw -= DeltaDistanceCurve;
				}
				else
				{
					RotateYaw += DeltaDistanceCurve;
				}

				float AbsRotateYawOffset = FMath::Abs(RotateYaw);
				if (AbsRotateYawOffset > 0.f)
				{
					float YawExcess = AbsRotateYawOffset - 90.f;
					if (RotateYaw > 0.f)
					{
						RotateYaw -= YawExcess;
					}
					else
					{
						RotateYaw += YawExcess;
					}

				}


			}
		}

	
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
