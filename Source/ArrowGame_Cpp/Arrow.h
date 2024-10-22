// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UCLASS()
class ARROWGAME_CPP_API AArrow : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultRoot;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionBox;
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;
	
public:	
	// Sets default values for this actor's properties
	AArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};