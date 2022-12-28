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

	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InsigatorPawn, bool bMissionSuccess)
{
	if (InsigatorPawn)
	{
		if (SpectatingViewpointClass)
		{
            TArray<AActor*> ReturnedActors;
            UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);

            if (ReturnedActors.Num() > 0)
            {
                AActor* NewViewTarget = ReturnedActors[0];

				for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
				{
					APlayerController* PC = It->Get();
                    if (PC)
                    {
                        PC->SetViewTargetWithBlend(NewViewTarget, .5f, EViewTargetBlendFunction::VTBlend_Cubic);
                    }
				}
            }
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Spectating view class is null, update it so that we can change cameras when you beat the lvl."));
		}
	}
	AFPSGameState* GS = GetGameState<AFPSGameState>();
	if (GS)
	{
		GS->MulticastOnMissionComplete(InsigatorPawn, bMissionSuccess);
	}
	OnMissionCompleted(InsigatorPawn, bMissionSuccess);
}
