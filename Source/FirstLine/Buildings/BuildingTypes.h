#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "Buildings/BuildingBase.h"
#include "BuildingTypes.generated.h"

USTRUCT(BlueprintType)
struct FIRSTLINE_API FBuildingData : public FTableRowBase
{
    GENERATED_BODY()

    // The building class to spawn
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building")
    TSubclassOf<ABuildingBase> BuildingClass;

    // Resource costs
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building")
    TMap<FGameplayTag, float> ResourceCosts;

    // Construction time in seconds
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building")
    float ConstructionTime = 10.0f;

    // Display name for UI
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building|UI")
    FText DisplayName;

    // Description for UI
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building|UI")
    FText Description;

    FBuildingData()
        : ConstructionTime(10.0f)
    {
    }
}; 