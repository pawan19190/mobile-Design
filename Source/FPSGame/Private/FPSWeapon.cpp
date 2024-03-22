// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSWeapon.h"
#include "FPSProjectile.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AFPSWeapon::AFPSWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunMesh1P = CreateAbstractDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh1P"));
	GunMesh3P = CreateAbstractDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh3P"));

	RootComponent = GunMesh1P;
	RootComponent = GunMesh3P;

	SetReplicates(true);

}

// Called when the game starts or when spawned
void AFPSWeapon::BeginPlay()
{
	Super::BeginPlay();
	

}




//////////////////////////////////////////////////////////////////////////
// Weapon usage
void AFPSWeapon::Fire()
{
	
	// local client will notify server
	if (Role < ROLE_Authority)
	{
		// try and play the sound if specified
		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		ServerFire();
		return;
	}

	// try and play the sound if specified
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and fire a projectile
	if (ProjectileClass)
	{
		
	FVector MuzzleLocation = GunMesh1P->GetSocketLocation("Muzzle");
	FRotator MuzzleRotation = GunMesh1P->GetSocketRotation("Muzzle");

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = 
	ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	ActorSpawnParams.Owner = GetOwner();
	
	// spawn the projectile at the muzzle
	GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
	}
	

	
	
}



//Server Fire
void AFPSWeapon::ServerFire_Implementation()
{
	Fire();

	

}

bool AFPSWeapon::ServerFire_Validate()
{
	return true;
}




// Called every frame
void AFPSWeapon::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

}

