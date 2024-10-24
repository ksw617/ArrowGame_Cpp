// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

AEnemyAIController::AEnemyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_Enemy.BT_Enemy'"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_Enemy.BB_Enemy'"));
	if (BB.Succeeded())
	{
		BlackboardData = BB.Object;
	}

}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	UE_LOG(LogTemp, Log, TEXT("OnPossess"));
	Super::OnPossess(InPawn);
	if (RunBehaviorTree(BehaviorTree))
	{
		UE_LOG(LogTemp, Log, TEXT("Run Behavior Tree"));

	}
}

void AEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
