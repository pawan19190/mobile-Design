// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "FPSWeapon.h"
#include "Net/UnrealNetwork.h"




AFPSCharacter::AFPSCharacter()
{
	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->RelativeLocation = FVector(0, 0, BaseEyeHeight); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1PComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh1PComponent->SetupAttachment(CameraComponent);
	Mesh1PComponent->CastShadow = false;
	Mesh1PComponent->RelativeRotation = FRotator(2.0f, -15.0f, 5.0f);
	Mesh1PComponent->RelativeLocation = FVector(0, 0, -160.0f);


	WeaponAttachSocketName3P = "WeaponSocket";
	WeaponAttachSocketName1P = "GripPoint";

	Health = 100;
	bDied = false;
}


////////////////////////////////////////////////////////////////////////////////////
//Called when the game starts or when spawned.
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();


	if (Role == ROLE_Authority)
	{
		//Spawn a default Weapon
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		TArray<USkeletalMeshComponent*> StaticComps;

		CurrentWeapon = GetWorld()->SpawnActor<AFPSWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		CurrentWeapon->GetComponents<USkeletalMeshComponent>(StaticComps);


		if (CurrentWeapon)
		{
			CurrentWeapon->SetOwner(this);

			StaticComps[1]->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName3P);
			StaticComps[0]->AttachToComponent(Mesh1PComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName1P);

		}

	}
	
}





//////////////////////////////////////////////////////////////////////////
// Input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}


//////////////////////////////////////////////////////////////////////////
// Movement
void AFPSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}


void AFPSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}


//////////////////////////////////////////////////////////////////////////
// Weapon usage
void AFPSCharacter::Fire()
{

	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}

	// try and play a firing animation if specified
	if (FireAnimation)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
		}
	}

}


//////////////////////////////////////////////////////////////////////////
// Damage & death

void AFPSCharacter::DestroySelf()
{


	if (Role == ROLE_Authority)
	{
		Destroy();
		CurrentWeapon->Destroy();
	}

	
	DetachFromControllerPendingDestroy();

	//Tipp: If you want to add a Death Animation Use SetLifeSpan and Delete Destroy() 
	//SetLifeSpan(10.0f);
}

float AFPSCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= Damage;
	UE_LOG(LogTemp, Warning, TEXT("Health: %s"), *FString::SanitizeFloat(Health));
	if (Health <= 0.0f)
	{
		bDied = true;
		
		

		DestroySelf();
		
		
		
	}
	return 0.0f;

}


FRotator AFPSCharacter::GetAimOffsets() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}



//////////////////////////////////////////////////////////////////////////
// Replication

void AFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSCharacter, CurrentWeapon);
	DOREPLIFETIME(AFPSCharacter, Health);
	DOREPLIFETIME(AFPSCharacter, bDied);
}




void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsLocallyControlled())
	{
		FRotator NewRot = CameraComponent->RelativeRotation;
		NewRot.Pitch = RemoteViewPitch * 360.0f / 255.0f;


		CameraComponent->SetRelativeRotation(NewRot);
	}

}
