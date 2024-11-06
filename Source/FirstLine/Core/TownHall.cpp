// Fill out your copyright notice in the Description page of Project Settings.


#include "TownHall.h"
#include "TimerManager.h"

// Sets default values
ATownHall::ATownHall()
{
    ProductionTime = 5.0f;
    PeasantCount = 0;
    bIsProducing = false;

    // This actor will call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATownHall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATownHall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATownHall::StartProduction()
{
    if (!bIsProducing)
    {
        bIsProducing = true;
        GetWorld()->GetTimerManager().SetTimer(ProductionTimerHandle, this, &ATownHall::FinishProduction, ProductionTime, false);
    }
}

void ATownHall::FinishProduction()
{
    bIsProducing = false;
    PeasantCount++;
}