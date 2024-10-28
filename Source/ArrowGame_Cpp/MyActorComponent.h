// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARROWGAME_CPP_API UMyActorComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Info", Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(EditAnywhere, Category = "Info", Meta = (AllowPrivateAccess = true))
	int32 MaxHp;

	UPROPERTY(EditAnywhere, Category = "Info", Meta = (AllowPrivateAccess = true))
	int32 Hp;
public:
	int32 GetLevel() const { return Level; }
	int32 GetHp() const { return Hp; }

public:	
	// Sets default values for this component's properties
	UMyActorComponent();

protected:
	virtual void InitializeComponent() override;
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	void SetLevel(int32 Lv);
	void OnDamaged(float DamageAmount);

		
};
