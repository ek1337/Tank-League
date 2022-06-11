// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	RootComponent = ProjectileMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<
		UProjectileMovementComponent>(TEXT("Projectile Movement Comp"));
	ProjectileMovementComponent->InitialSpeed = 2000.f;
	ProjectileMovementComponent->MaxSpeed = 7000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.2f;

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	TrailParticles->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			LaunchSound,
			GetActorLocation(),
			GetActorRotation());
	}
}

// note: Mesh component (such as Projectile Mesh) has to have Collision Enabled!
void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("%s Component is hitting the %s Actor and it's Component %s"),
	       *HitComp->GetName(),
	       *OtherActor->GetName(),
	       *OtherComp->GetName());

	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr)
	{
		Destroy();
		return;
	}

	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,
			Damage,
			MyOwnerInstigator,
			this,
			DamageTypeClass);

		if (HitParticles)
			UGameplayStatics::SpawnEmitterAtLocation(
				this,
				HitParticles,
				GetActorLocation(),
				GetActorRotation());
	}
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			GetActorLocation(),
			GetActorRotation());
	}
	if (HitCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
	}
	
	Destroy();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
