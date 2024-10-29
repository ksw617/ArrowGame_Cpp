// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyAIController.h"
#include "EnemyAnimInstance.h"
#include "MyActorComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonAurora/Characters/Heroes/Aurora/Meshes/Aurora.Aurora'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));

	}


	static ConstructorHelpers::FClassFinder<UAnimInstance> AI(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/ABP_Enemy.ABP_Enemy_C'"));
	if (AI.Succeeded())
	{
		GetMesh()->SetAnimClass(AI.Class);

	}

	AIControllerClass = AEnemyAIController::StaticClass();


	MyActorComponent = CreateDefaultSubobject<UMyActorComponent>(FName("MyActorComponent"));

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyAnimInstace = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	EnemyAnimInstace->OnMontageEnded.AddDynamic(this, &AEnemy::OnAttackMontageEnded);
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::Attack()
{
	if (!IsAttacking)
	{
		IsAttacking = true;
		
		if (IsValid(EnemyAnimInstace))
		{
			EnemyAnimInstace->PlayAttackMontage();

		}
	}

}

void AEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
}

