// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole.h"

#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("AbbaSceneComp"));
	RootComponent = SceneComp;

	AttractSphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("AttractSphere"));
	AttractSphereCol->SetupAttachment(SceneComp);

	DestroySphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("DestroySphere"));
	DestroySphereCol->SetupAttachment(SceneComp);

}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    TArray<AActor*> actorArray;
    AttractSphereCol->GetOverlappingActors(actorArray);
    for (int i = 0; i < actorArray.Num(); ++i)
    {
        if (actorArray[i] == this)
            continue;

        UStaticMeshComponent* SM = Cast<UStaticMeshComponent>(actorArray[i]->GetComponentByClass(UStaticMeshComponent::StaticClass()));
        if (SM)
        {
            SM->AddRadialForce(GetActorLocation(), AttractSphereCol->GetScaledSphereRadius(), -5000000, RIF_Linear);
        }
    }


	TArray<AActor*> actorsToDestroy;
    DestroySphereCol->GetOverlappingActors(actorsToDestroy);
    for (int i = 0; i < actorsToDestroy.Num(); ++i)
    {
		if (actorsToDestroy[i] != this)
			actorsToDestroy[i]->Destroy();
    }
}

