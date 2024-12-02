#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IConstructable.h"
#include "BuildingBase.generated.h"

class UBoxComponent;

UENUM(BlueprintType)
enum class EBuildingState : uint8
{
    Placing,    // Ghost/preview state
    Building,   // Under construction
    Complete    // Construction finished
};

UCLASS(Abstract)
class FIRSTLINE_API ABuildingBase : public AActor, public IConstructable
{
    GENERATED_BODY()

public:
    ABuildingBase();

    // IConstructable interface
    virtual bool CanPlaceAt_Implementation(const FVector& Location) const override;
    virtual void StartConstruction_Implementation(APawn* Worker) override;
    virtual void UpdateConstruction_Implementation(float DeltaTime) override;
    virtual void WorkerRemoved_Implementation(APawn* Worker) override;

    UFUNCTION(BlueprintCallable,BlueprintPure)
    bool CanBePlaced() { return bCanBePlaced;}

    // Blueprint implementable events for visual feedback
    UFUNCTION(BlueprintImplementableEvent, Category = "Construction")
    void OnConstructionStateChanged(EBuildingState NewState);
    void SetBuildingState(EBuildingState NewBuildingState);

protected:

    UPROPERTY(EditDefaultsOnly, Category = "Construction")
    TMap<FGameplayTag, float> ResourceCosts;

    UPROPERTY(EditDefaultsOnly, Category = "Construction")
    float ConstructionTime;

    UPROPERTY(EditDefaultsOnly, Category = "Construction")
    UStaticMeshComponent* BuildingMesh;

    UPROPERTY(ReplicatedUsing = OnRep_BuildingState)
    EBuildingState BuildingState;

    UPROPERTY(Replicated)
    float ConstructionProgress;

    UPROPERTY(Replicated)
    APawn* AssignedWorker;

    UFUNCTION()
    void OnRep_BuildingState();

    // Collision box for placement validation
    UPROPERTY(EditDefaultsOnly, Category = "Construction")
    UBoxComponent* PlacementCollision;

    // Whether the current location is valid for placement
    UPROPERTY(BlueprintReadOnly, Category = "Construction")
    bool bCanBePlaced;

    // Called when placement collision overlaps
    UFUNCTION()
    void OnPlacementOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnPlacementOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
}; 