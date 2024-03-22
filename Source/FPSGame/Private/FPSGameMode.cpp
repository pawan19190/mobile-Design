// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"



AFPSGameMode::AFPSGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

}


void AFPSGameMode::RespawnDeadPlayers()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		
		

		if (PC && PC->GetPawn() == nullptr)
		{
			RestartPlayer(PC);
		}
	}

	
}



void AFPSGameMode::BeginPlay()
{
	//Respawn all dead players every 5s
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AFPSGameMode::RespawnDeadPlayers, 5.0f, true);
}

void AFPSGameMode::Tick(float DeltaTime)
{
	
	
}