#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "BuildingTargetActor.generated.h"

class ABuildingBase;
class ABuildingVisualizationReticle;
class UMaterialInterface;

UCLASS()
class FIRSTLINE_API ABuildingTargetActor : public AGameplayAbilityTargetActor
{
    GENERATED_BODY()

public:
    ABuildingTargetActor();

    virtual void StartTargeting(UGameplayAbility* Ability) override;
    virtual void ConfirmTargetingAndContinue() override;
    virtual void CancelTargeting() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void SetBuildingClass(TSubclassOf<ABuildingBase> InBuildingClass);

protected:
    // The building class to place
    UPROPERTY()
    TSubclassOf<ABuildingBase> BuildingClass;

    // Box component for collision checking
    UPROPERTY()
    class UBoxComponent* PlacementCollision;

    // The reticle actor for visualization (client-only)
    UPROPERTY()
    ABuildingVisualizationReticle* BuildingReticle;

    // Material for valid placement
    UPROPERTY(EditDefaultsOnly, Category = "Visualization")
    UMaterialInterface* ValidPlacementMaterial;

    // Material for invalid placement
    UPROPERTY(EditDefaultsOnly, Category = "Visualization")
    UMaterialInterface* InvalidPlacementMaterial;

    // Whether the current location is valid for placement
    bool bCanPlaceHere;

    // Last trace result for placement
    FHitResult LastHitResult;

    // Creates target data for the building placement
    virtual FGameplayAbilityTargetDataHandle MakeTargetData(const FHitResult& HitResult) const;

    // Performs ground trace and updates placement validity
    void UpdatePlacement();

    // Checks if the current location is valid for placement
    bool IsValidPlacementLocation(const FHitResult& HitResult) const;

    // Creates and initializes the building reticle
    void SetupBuildingReticle();

    // Helper to check if this is the locally controlled instance
    bool IsLocallyControlled() const;
}; 