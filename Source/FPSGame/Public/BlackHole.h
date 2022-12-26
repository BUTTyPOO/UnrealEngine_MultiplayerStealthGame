// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackHole.generated.h"

UCLASS()
class FPSGAME_API ABlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlackHole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	class USphereComponent* AttractSphereCol;

	UPROPERTY(EditAnywhere)
	class USphereComponent* DestroySphereCol;

    UPROPERTY(EditAnywhere)
    class USceneComponent* SceneComp;

public:	
	virtual void Tick(float DeltaTime) override;

};
