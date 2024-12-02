#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "IConstructable.generated.h"

UINTERFACE(MinimalAPI)
class UConstructable : public UInterface
{
    GENERATED_BODY()
};

class FIRSTLINE_API IConstructable
{
    GENERATED_BODY()

public:
    // Check if building can be placed at location
    UFUNCTION(BlueprintNativeEvent, Category = "Construction")
    bool CanPlaceAt(const FVector& Location) const;

    // Called when construction starts
    UFUNCTION(BlueprintNativeEvent, Category = "Construction")
    void StartConstruction(APawn* Worker);

    // Called when construction progresses
    UFUNCTION(BlueprintNativeEvent, Category = "Construction")
    void UpdateConstruction(float DeltaTime);

    // Called when worker is removed/destroyed
    UFUNCTION(BlueprintNativeEvent, Category = "Construction")
    void WorkerRemoved(APawn* Worker);
}; 