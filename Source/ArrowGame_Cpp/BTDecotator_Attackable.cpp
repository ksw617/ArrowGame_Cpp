// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecotator_Attackable.h"
#include "AIController.h" 
#include "Archer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecotator_Attackable::UBTDecotator_Attackable()
{
	NodeName = TEXT("Attackable");
}

bool UBTDecotator_Attackable::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool Result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (Pawn == nullptr)
		return false;

	auto Target = Cast<AArcher>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")));
	if (Target == nullptr)
		return false;

	if (Target->GetDistanceTo(Pawn) > 200.f)
		return false;

	//Result == true
	return Result;
}
