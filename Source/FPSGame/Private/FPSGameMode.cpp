// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include "Kismet/GameplayStatics.h"

#include "FPSGameState.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	// custom game state
	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bMissionSuccess)
{

	if (InstigatorPawn) {

		// InstigatorPawn->DisableInput(nullptr); // to replicate

		if (SpectatingViewpointClass) {

			TArray<AActor*> ReturnedActors;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);

			// change view target if any valid actor found
			if (ReturnedActors.Num() > 0) {

				AActor* NewViewTarget = ReturnedActors[0];

				for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
				{

					APlayerController* PC = It->Get();
					if (PC) {

						PC->SetViewTargetWithBlend(NewViewTarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);

					}

				}

			}

		}
		else {

			UE_LOG(LogTemp, Warning, TEXT("SpectatingViewpointClass in nullptr. Please update GameMode class with valid subclass. Cannot change spectating view target on mission complete."));

		}
		
	}

	AFPSGameState* GS = GetGameState<AFPSGameState>();
	if (GS)
	{
		GS->MulticastOnMissionComplete(InstigatorPawn, bMissionSuccess);
	}


	OnMissionCompleted(InstigatorPawn, bMissionSuccess); // to repcicate


}
