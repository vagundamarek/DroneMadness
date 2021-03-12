// Fill out your copyright notice in the Description page of Project Settings.

#include "DroneControl.h"
#include "Components/SphereComponent.h"

UDroneControl::UDroneControl()
{
    PrimaryComponentTick.bCanEverTick = true;
}

FVector UDroneControl::GetCurrentCommand() const
{
    return CurrentCommand;
}

void UDroneControl::BeginDestroy()
{
    Super::BeginDestroy();
    DroneUnregister.Broadcast(this);
}

void UDroneControl::BeginPlay()
{
    Super::BeginPlay();
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UDroneControl::GenerateCommand, Interval, true);
}

void UDroneControl::GenerateCommand()
{
    CurrentCommand = { FMath::RandRange(-200.f, 200.f), FMath::RandRange(-200.f, 200.f), 0 };
    SendDroneCommand.Broadcast();
}

void UDroneControl::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
