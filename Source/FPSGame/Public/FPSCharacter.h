// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class AFPSProjectile;

class UAnimSequence;
class AFPSWeapon;


UCLASS()
class AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Pawn mesh: 1st person view  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
	USkeletalMeshComponent* Mesh1PComponent;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

public:
	AFPSCharacter();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AFPSProjectile> ProjectileClass;

	/* Retrieve Pitch/Yaw from current camera */
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	FRotator GetAimOffsets() const;
	

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UAnimSequence* FireAnimation;


	/** Take damage, handle death */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/** setup pawn specific input handlers */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	/** currently equipped weapon */
	UPROPERTY(Replicated)
	AFPSWeapon* CurrentWeapon;

	/** socket or bone name for attaching weapon mesh Third Person*/
	UPROPERTY(VisibleDefaultsOnly, Category = "WeaponAttachPoint")
	FName WeaponAttachSocketName3P;

	/** socket or bone name for attaching weapon mesh First Person*/
	UPROPERTY(VisibleDefaultsOnly, Category = "WeaponAttachPoint")
	FName WeaponAttachSocketName1P;
	
	/** default Weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AFPSWeapon> StarterWeaponClass;

	/** Current health of the Pawn */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	int32 Health;

	/** Identifies if pawn is Died */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	bool bDied;
	
	/** call Fire */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();
	/** DestroySelf */
	UFUNCTION(BlueprintCallable, Category = "Player")
	void DestroySelf();

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1PComponent; }

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComponent; }
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

