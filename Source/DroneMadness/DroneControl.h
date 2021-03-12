// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "DroneControl.generated.h"

class ADrone;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSendDroneCommand);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDroneUnregistrer, UDroneControl*, RemovedControl);

/*
Drone Control component. Actor needs to have a collision object for drones to overlap with.
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class DRONEMADNESS_API UDroneControl : public UActorComponent 
{
    GENERATED_BODY()

public:
    UDroneControl();
    FVector GetCurrentCommand() const;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY()
    FSendDroneCommand SendDroneCommand;

    UPROPERTY()
    FDroneUnregistrer DroneUnregister;

protected:
    virtual void BeginDestroy();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void GenerateCommand();

    FTimerHandle TimerHandle;

    UPROPERTY(Category = CommandInterval, EditAnywhere, BlueprintReadWrite)
    float Interval;

    FVector CurrentCommand;
};
