// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TownHall.generated.h"

UCLASS()
class FIRSTLINE_API ATownHall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATownHall();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Production time in seconds
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production")
    float ProductionTime;

    // Current number of peasants produced
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Production")
    int32 PeasantCount;

    // Flag to check if production is ongoing
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Production")
    bool bIsProducing;


    // Function to start the production process
    UFUNCTION(BlueprintCallable, Category = "Production")
    void StartProduction();

    // Function to finish the production process
    UFUNCTION()
    void FinishProduction();

    // Timer handle for production timing
    FTimerHandle ProductionTimerHandle;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
