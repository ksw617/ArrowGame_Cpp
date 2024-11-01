// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActorComponent.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h" // UGameplayStatics 사용하기 위해서


UMyActorComponent::UMyActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	Level = 1;

}


void UMyActorComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetLevel(Level);
}

void UMyActorComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UMyActorComponent::SetLevel(int32 Lv)
{
	//auto MyGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance)
	{
		auto CharacterData = MyGameInstance->GetCharacterData(Lv);
		if (CharacterData)
		{
			Level = CharacterData->Level;
			MaxHp = CharacterData->MaxHp;
			Hp = MaxHp;
		
		}

	}
}

void UMyActorComponent::OnDamaged(float DamageAmount)
{

	int32 NewHp = Hp - DamageAmount;
	SetHp(NewHp);

	
}

void UMyActorComponent::SetHp(int32 NewHp)
{
	Hp = NewHp;
	if (Hp < 0)
	{
		Hp = 0;

	}

	OnHpChanged.Broadcast();
}

float UMyActorComponent::GetHpRatio()
{
	if (MaxHp == 0|| Hp == 0)
	{
		return 0.f;

	}
	return (float)Hp / (float)MaxHp;
}

