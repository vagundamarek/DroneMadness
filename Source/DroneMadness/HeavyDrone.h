// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Drone.h"
#include "HeavyDrone.generated.h"

UCLASS()

class DRONEMADNESS_API AHeavyDrone : public ADrone 
{
    GENERATED_BODY()
    AHeavyDrone(const FObjectInitializer& Initializer);

protected:
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

    void GenerateCommand() override;

    bool bIsDamaged;
};
