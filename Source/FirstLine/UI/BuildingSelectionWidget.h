#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Buildings/BuildingTypes.h"
#include "BuildingSelectionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildingSelected, const FName&, BuildingDataRowName);

UCLASS(Abstract)
class FIRSTLINE_API UBuildingSelectionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    
    // Event when a building is selected
    UPROPERTY(BlueprintAssignable, Category = "Building")
    FOnBuildingSelected OnBuildingSelected;

protected:
    // The data table containing building definitions
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn=true), Category = "Building")
    UDataTable* BuildingDataTable;

    // Called when player clicks a building option
    UFUNCTION(BlueprintCallable, Category = "Building")
    void SelectBuilding(FName BuildingRowName);
}; 