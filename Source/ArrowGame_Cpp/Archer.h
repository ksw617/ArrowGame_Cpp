// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Archer.generated.h"

UCLASS()
class ARROWGAME_CPP_API AArcher : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere)
	class UPlayerAnimInstance* PlayerAnimInstance; // Ãß°¡

public:
	// Sets default values for this character's properties
	AArcher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	void KeyUpDown(float Value);
	void KeyLeftRight(float Value);
	void MouseLookLeftRight(float Value);
	void MouseLookUpDown(float Value);
	void Fire();

};
