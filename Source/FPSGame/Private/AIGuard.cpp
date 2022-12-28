// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGuard.h"

#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AAIGuard::AAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
}

// Called when the game starts or when spawned
void AAIGuard::BeginPlay()
{
	Super::BeginPlay();
	OriginalRotation = GetActorRotation();
    PawnSensingComp->OnSeePawn.AddDynamic(this, &AAIGuard::OnPawnSeen);
    PawnSensingComp->OnHearNoise.AddDynamic(this, &AAIGuard::OnHearNoise);
	SetGuardState(EAIState::Idle);
}

void AAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (!SeenPawn) return;

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32, 12, FColor::Red, false, 10);
    UE_LOG(LogTemp, Display, TEXT("OVERLAPPED WITH EXTRACTION ZONE"));

    AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
    if (GM)
    {
        GM->CompleteMission(SeenPawn, false);
    }
	SetGuardState(EAIState::Alerted);
}

void AAIGuard::OnHearNoise(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted) return;

	DrawDebugSphere(GetWorld(), Location, 16, 12, FColor::Green, false, 10);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
    NewLookAt.Pitch = 0;
    NewLookAt.Roll = 0;
	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AAIGuard::ResetOrientation, 3.0f);
	SetGuardState(EAIState::Sus);
}

void AAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted) return;
	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::Idle);
}

void AAIGuard::OnRep_GuardState()
{
	OnStateChanged(GuardState);
}

void AAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
		return;

	GuardState = NewState;
	OnRep_GuardState(); 
}

// Called every frame
void AAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAIGuard, GuardState);
}