// Fill out your copyright notice in the Description page of Project Settings.

#include "Drone.h"
#include "Components/StaticMeshComponent.h"
#include "DroneControl.h"
#include "DroneMadnessProjectile.h"
#include "Math/UnrealMathUtility.h"

ADrone::ADrone(const FObjectInitializer& Initializer)
    : Super(Initializer)
{
    MeshComponent = Initializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh Component"));
    MeshComponent->SetupAttachment(RootComponent);
    RootComponent = MeshComponent;

    Controller = nullptr;
    bIsInitialized = false;
    bIsLost = true;

    CurrentSpeed = 0;
    MovementVector = { 0, 0, 0 };
    TargetLocation = { 0, 0, 0 };
    MovementCommand = { 0, 0, 0 };

    PrimaryActorTick.bCanEverTick = true;
}

void ADrone::BeginPlay()
{
    Super::BeginPlay();
    MeshComponent->BodyInstance.SetCollisionProfileName("Drone");
    MeshComponent->OnComponentHit.AddDynamic(this, &ADrone::OnHit);
    MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ADrone::OnActorBeginOverlap);
    MeshComponent->OnComponentEndOverlap.AddDynamic(this, &ADrone::OnActorEndOverlap);
}

void ADrone::CalculateMovementVector()
{
    TargetLocation = GetActorLocation() + MovementCommand;
    MovementVector = CurrentSpeed / MovementCommand.Size() * MovementCommand;
}

void ADrone::GenerateCommand()
{
    MovementCommand = { FMath::RandRange(-300.f, 300.f), FMath::RandRange(-300.f, 300.f), 0 };
    CalculateMovementVector();
}

void ADrone::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if ((OtherActor != nullptr) && (OtherActor->IsA<ADroneMadnessProjectile>()))
        Destroy();
}

void ADrone::NewCommand()
{
    MovementCommand = Controller->GetCurrentCommand();
    CalculateMovementVector();
}

void ADrone::RemoveControl(UDroneControl* RemovedControl)
{
    //UE_LOG(LogTemp, Display, TEXT("RemoveControl(%s, &ADrone::NewCommand, %u"), *GetName(), Controller->GetUniqueID());
    OverLapsAfterSpawn.Remove(RemovedControl->GetOwner());
    if (RemovedControl == Controller) 
    {
        SetRemoveControlDynamics(true);
        SetNewControlFromArray();
    }
    else
        Controller->DroneUnregister.RemoveDynamic(this, &ADrone::RemoveControl);
}

void ADrone::SetNewControlFromArray()
{
    SetNewCommandDynamics(false);

    bIsLost = true;
    if (OverLapsAfterSpawn.Num() != 0) 
    {
        for (auto Actor : OverLapsAfterSpawn) 
        {
            auto* OtherController = Actor->FindComponentByClass<UDroneControl>();
            {
                OnNewCommandCenter();
                Controller = OtherController;
                SetNewCommandDynamics(true);
                bIsLost = false;
                return;
            }
        }
    }

    Controller = nullptr;
}

void ADrone::SetNewCommandDynamics(bool value)
{
    if (value) 
    {
        //UE_LOG(LogTemp, Display, TEXT("AddDynamic(%s, &ADrone::NewCommand, %u"), *GetName(), Controller->GetUniqueID());
        Controller->SendDroneCommand.AddDynamic(this, &ADrone::NewCommand);
    }
    else 
    {
       //UE_LOG(LogTemp, Display, TEXT("RemoveDynamic(%s,  &ADrone::NewCommand, %u"), *GetName(), Controller->GetUniqueID());
        Controller->SendDroneCommand.RemoveDynamic(this, &ADrone::NewCommand);
    }
}
void ADrone::SetRemoveControlDynamics(bool value)
{
    if (value) 
    {
        //UE_LOG(LogTemp, Display, TEXT("AddDynamic(%s,  &ADrone::RemoveControl, %u"), *GetName(), Controller->GetUniqueID());
        Controller->DroneUnregister.AddDynamic(this, &ADrone::RemoveControl);
    }
    else 
    {
        //UE_LOG(LogTemp, Display, TEXT("RemoveDynamic(%s,  &ADrone::RemoveControl, %u"), *GetName(), Controller->GetUniqueID());
        Controller->DroneUnregister.RemoveDynamic(this, &ADrone::RemoveControl);
    }
}

void ADrone::InitDrone(UDroneControl* ParentDroneController)
{
    //UE_LOG(LogTemp, Warning, TEXT("INIT, %s"), *GetName());
    CurrentSpeed = MaxSpeed;
    bIsInitialized = true;
    if (ParentDroneController != nullptr) 
    {
        bIsLost = false;
        Controller = ParentDroneController;
        SetRemoveControlDynamics(true);
        SetNewCommandDynamics(true);
    }
    else
        GenerateCommand();
}

void ADrone::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor == nullptr)
        return;
    auto* OtherController = OtherActor->FindComponentByClass<UDroneControl>();
    if (OtherController != nullptr && OverlappedComp == MeshComponent)
    {

        //UE_LOG(LogTemp, Warning, TEXT("OnActorBeginOverlap, %s"), *GetName());
        if (bIsInitialized) {

            if (OtherController != Controller) 
            {
                if (Controller != nullptr) 
                {
                    SetNewCommandDynamics(false);
                }
                OnNewCommandCenter();

                Controller = OtherController;
                SetNewCommandDynamics(true);

                if (OverLapsAfterSpawn.Find(OtherActor) == INDEX_NONE)
                    SetRemoveControlDynamics(true);
            }
        }
        //UE_LOG(LogTemp, Display, TEXT("Add to array %u"), OtherController->GetUniqueID());
        OverLapsAfterSpawn.Add(OtherActor);

        bIsLost = false;
    }
}

void ADrone::OnActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

    if ((OtherActor != nullptr) && (OtherActor->FindComponentByClass<UDroneControl>() != nullptr) && OverlappedComp == MeshComponent) 
    {
        //UE_LOG(LogTemp, Warning, TEXT("OnActorEndOverlap, %s"), *GetName());
        //UE_LOG(LogTemp, Display, TEXT("Remove from array %u"), Controller->GetUniqueID());
        OverLapsAfterSpawn.RemoveSingle(OtherActor);

        if (OverLapsAfterSpawn.Find(OtherActor) == INDEX_NONE) 
        {
            SetRemoveControlDynamics(false);
            SetNewControlFromArray();
        }
    }
}

void ADrone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!TargetLocation.Equals(GetActorLocation(), 5))
        SetActorLocation(GetActorLocation() + (MovementVector)*DeltaTime);
    else
        if (bIsLost)
        {
        GenerateCommand();
        }
}
