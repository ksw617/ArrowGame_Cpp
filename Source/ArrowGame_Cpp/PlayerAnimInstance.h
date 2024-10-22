// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARROWGAME_CPP_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float Speed;
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool ShouldMove; 
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool IsFalling;
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FVector Velocity;
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float YawOffset;
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FRotator AimRotation;
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float RotateYaw;
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FRotator MovingRotation;
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FRotator PrevRotation;
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float PrevDistanceCurve;
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float DistanceCurve;
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float DeltaDistanceCurve;

public:
	UPROPERTY(VisibleAnywhere)
	class AArcher* Archer;
	UPROPERTY(VisibleAnywhere)
	class UCharacterMovementComponent* CharacterMovement;
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* FireMontage;

public:
	UPlayerAnimInstance();
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:
	void PlayFireMontage();
	
};
