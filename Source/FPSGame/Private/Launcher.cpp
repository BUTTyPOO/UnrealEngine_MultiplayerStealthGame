// Fill out your copyright notice in the Description page of Project Settings.


#include "Launcher.h"

#include "Components/BoxComponent.h"
#include "FPSCharacter.h"
#include "Particles\ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALauncher::ALauncher()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>("Box Col");
    BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ALauncher::HandleOverlap);
	RootComponent = BoxComp;
}

void ALauncher::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    FVector LaunchVec = GetActorForwardVector() * 500 + FVector(0, 0, 1000);
	AFPSCharacter* Player = Cast<AFPSCharacter>(OtherActor);
	if (Player)
	{
		Player->LaunchCharacter(LaunchVec, true, true);
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(LaunchVec*500);
	}
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, GetActorLocation());
}
