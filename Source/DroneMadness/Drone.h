// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Drone.generated.h"

class UStaticMeshComponent;
class UDroneControl;

/*
* Class for basic drone. Also handles logic for registering drones to command centers. Currently only works if spawned from command center.
*/
UCLASS()
class DRONEMADNESS_API ADrone : public AActor 
{
    GENERATED_BODY()

public:
    ADrone(const FObjectInitializer& Initializer);

    void InitDrone(UDroneControl* DroneController);

    UFUNCTION(BlueprintImplementableEvent)
    void OnNewCommandCenter();

protected:
    virtual void BeginPlay() override;

    void CalculateMovementVector();
    virtual void GenerateCommand();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* MeshComponent;

    UFUNCTION()
    virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
    virtual void OnActorBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    virtual void OnActorEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    UFUNCTION()

    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void NewCommand();

    UFUNCTION()
    void RemoveControl(UDroneControl* RemovedControl);

    UPROPERTY(Category = Move, EditAnywhere, BlueprintReadWrite)
    float MaxSpeed;

    UPROPERTY(Category = Move, EditAnywhere, BlueprintReadOnly)
    float CurrentSpeed;

    UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadOnly)
    bool bIsLost;

    FVector MovementVector;
    FVector MovementCommand;
    FVector TargetLocation;

    bool bIsInitialized;
    UDroneControl* Controller;

private:
    TArray<AActor*> OverLapsAfterSpawn;
    void SetNewControlFromArray();

    void SetNewCommandDynamics(bool value);
    void SetRemoveControlDynamics(bool value);
};
