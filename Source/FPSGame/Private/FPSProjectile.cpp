// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "FPSCharacter.h"

AFPSProjectile::AFPSProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	//Replication
	SetReplicates(true);
	SetReplicateMovement(true);

	
}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();

	bool OwnedLocally = IsOwnedBy(UGameplayStatics::GetPlayerController(this, 0));
}


//Projectile Hit Character
void AFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AFPSCharacter* Char = Cast<AFPSCharacter>(OtherActor);


	//Tip: Click Window -> Developer Tools, then you will see Output Log!
	if (Char == nullptr)
	{
		//Write to Output Log
		UE_LOG(LogTemp, Warning, TEXT("No Char"));
	} 
	else
	{
		//Write to Output Log
		UE_LOG(LogTemp, Warning, TEXT("Char"));
	}
	
	TSubclassOf<UDamageType> P;
	FHitResult HitInfo;

	//Only ApplyPointDamage if we hit a other character
	if (GetOwner() != Char)
	{

		if (Role == ROLE_Authority)
		{
			UGameplayStatics::ApplyPointDamage(OtherActor, 20, GetActorLocation(), HitInfo, nullptr, this, P);

		}

		//RadialDamage destroys the cubes and can affects Players
		UGameplayStatics::ApplyRadialDamage(GetWorld(), 1, GetActorLocation(), 100.0f, P, TArray<AActor*>() , this, (AController*)GetOwner(), true, ECC_Visibility);
	}
	


	
	
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		
		
	}

	if (Role == ROLE_Authority)
	{
		Destroy();
	}
}