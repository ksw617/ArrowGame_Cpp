// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class ARROWGAME_CPP_API AEnemy : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	class UEnemyAnimInstance* EnemyAnimInstace;
	UPROPERTY(VisibleAnywhere)
	class UMyActorComponent* MyActorComponent;
private:
	bool IsAttacking = false;
public:
	AEnemy();
public:
	bool GetIsAttacking() const { return IsAttacking; }

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void Attack();
public:
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
