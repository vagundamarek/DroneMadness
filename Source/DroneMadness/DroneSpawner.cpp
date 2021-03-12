// Fill out your copyright notice in the Description page of Project Settings.

#include "DroneSpawner.h"
#include "DroneControl.h"
#include "Kismet/GameplayStatics.h"

UDroneSpawner::UDroneSpawner()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UDroneSpawner::BeginPlay()
{
    Super::BeginPlay();
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UDroneSpawner::SpawnDrones, Interval, true);
    ParentDroneControl = GetOwner()->FindComponentByClass<UDroneControl>();
}

void UDroneSpawner::SpawnDrones() const
{
    for (int i = 0; i < Ammount; i++) 
    {
        ADrone* SpawnedDrone = GetWorld()->SpawnActor<ADrone>(DroneType, GetOwner()->GetTransform());
        if (SpawnedDrone)
            SpawnedDrone->InitDrone(ParentDroneControl);
    }
}

void UDroneSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
