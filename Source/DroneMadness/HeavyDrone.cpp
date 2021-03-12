// Fill out your copyright notice in the Description page of Project Settings.

#include "HeavyDrone.h"
#include "DroneMadnessProjectile.h"

AHeavyDrone::AHeavyDrone(const FObjectInitializer& Initializer)
    : ADrone(Initializer)
{
    bIsDamaged = false;
}

void AHeavyDrone::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if ((OtherActor != nullptr) && (OtherActor->IsA<ADroneMadnessProjectile>())) 
    {
        if (bIsDamaged)
            Destroy();
        else 
        {
            bIsDamaged = true;
            CurrentSpeed = 0.f;
            CalculateMovementVector();
        }
    }
}

void AHeavyDrone::GenerateCommand()
{
    if (!bIsDamaged)
        ADrone::GenerateCommand();
}
