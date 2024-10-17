// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "Archer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
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
		FVector Velocity = CharacterMovement->Velocity;
		FRotator Rotation = Archer->GetActorRotation();
		FVector UnrotateVector = Rotation.UnrotateVector(Velocity);

		Vertical = UnrotateVector.X;
		Horizontal = UnrotateVector.Y;

		Speed = Archer->GetVelocity().Size2D();

	}

}
