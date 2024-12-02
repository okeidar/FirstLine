#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityWorldReticle.h"
#include "GameFramework/Actor.h"
#include "BuildingPlacementReticle.generated.h"

class UCapsuleComponent;

UCLASS()
class FIRSTLINE_API ABuildingVisualizationReticle : public AGameplayAbilityWorldReticle
{
    GENERATED_BODY()

public:
    ABuildingVisualizationReticle(const FObjectInitializer& ObjectInitializer);

    void InitializeReticleVisualizationInformation(AActor* InTargetingActor, AActor* VisualizationActor, UMaterialInterface* VisualizationMaterial);
    
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
    UPROPERTY()
    UCapsuleComponent* CollisionComponent;

    UPROPERTY()
    TArray<UActorComponent*> VisualizationComponents;
};
