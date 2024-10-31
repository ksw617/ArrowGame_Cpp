// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AArrow::AArrow()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(FName("DefaultRoot"));
	SetRootComponent(DefaultRoot);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("/Script/Engine.StaticMesh'/Game/ParagonSparrow/FX/Meshes/Heroes/Sparrow/Abilities/SM_Sparrow_Arrow.SM_Sparrow_Arrow'"));

	if (SM.Succeeded())
	{
		Mesh->SetStaticMesh(SM.Object);
		Mesh->SetupAttachment(DefaultRoot);
		Mesh->SetRelativeLocationAndRotation(FVector(100.f, 0.f, 0.f), FRotator(90.f, 0.f, 0.f));
		Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	}

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("Collision Box"));
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAll"));
	CollisionBox->SetupAttachment(Mesh);
	CollisionBox->SetRelativeLocation(FVector(0.f, 0.f, -55.f));
	CollisionBox->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

	//Ãß°¡
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnOverlapBegin);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	ProjectileMovement->SetUpdatedComponent(DefaultRoot);
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonSparrow/FX/Particles/Sparrow/Abilities/Primary/FX/P_Sparrow_HitHero.P_Sparrow_HitHero'"));
	if (ParticleAsset.Succeeded())
	{
		HitParticleSystem = ParticleAsset.Object;
	}

}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArrow::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ProjectileMovement->StopMovementImmediately();
	ProjectileMovement->ProjectileGravityScale = 0;

	AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticleSystem, CollisionBox->GetComponentLocation(), FRotator::ZeroRotator, true);


	UGameplayStatics::ApplyDamage(OtherActor, 10.f, ProjectileMovement->GetOwner()->GetInstigatorController(), nullptr, NULL);


	FTimerHandle TimerHandler;
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, [&]() {Destroy(); }, 3.f, false);
}

