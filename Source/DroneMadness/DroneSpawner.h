// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Drone.h"
#include "DroneSpawner.generated.h"

class UDroneControl;

/*
Compomenet for spawning drones. Doesnt require DroneControl component to function.
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class DRONEMADNESS_API UDroneSpawner : public UActorComponent 
{
    GENERATED_BODY()

public:
    UDroneSpawner();
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    virtual void BeginPlay() override;
    void SpawnDrones() const;

    UPROPERTY(Category = DroneSpawner, EditAnywhere, BlueprintReadWrite)
    int Ammount;

    UPROPERTY(Category = DroneSpawner, EditAnywhere, BlueprintReadWrite)
    float Interval;

    FTimerHandle TimerHandle;

    UDroneControl* ParentDroneControl;

    UPROPERTY(Category = DroneSpawner, EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ADrone> DroneType;
};
