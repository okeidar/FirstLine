#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BuildingInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FBuildingInputAction
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    const class UInputAction* ConfirmAction;

    UPROPERTY(EditDefaultsOnly)
    const class UInputAction* CancelAction;
};

UCLASS()
class FIRSTLINE_API UBuildingInputConfig : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FBuildingInputAction BuildingInputs;
}; 