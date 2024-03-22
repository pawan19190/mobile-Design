// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSWeapon.generated.h"

class USkeletalMeshComponent;
class AFPSProjectile;
class AFPSCharacter;
class USoundBase;

UCLASS()
class FPSGAME_API AFPSWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSWeapon();

	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** weapon mesh: 1st person view */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* GunMesh1P;

	/** weapon mesh: 3rd person view */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* GunMesh3P;

	


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();





	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AFPSProjectile> ProjectileClass;
	
	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	USoundBase* FireSound;
	
};
