// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyActorComponent.generated.h"

 DECLARE_MULTICAST_DELEGATE(FOnHpChanged)

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
	UMyActorComponent();

public:
	FOnHpChanged OnHpChanged;

protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
public:
	void SetLevel(int32 Lv);
	void OnDamaged(float DamageAmount);

	//Ãß°¡
	void SetHp(int32 NewHp);
	float GetHpRatio();

		
};
