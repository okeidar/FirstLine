#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "FirstLine/Core/AbilitySystem/FirstLineGameplayTags.h"
#include "CommandTypes.generated.h"

USTRUCT(BlueprintType)
struct FIRSTLINE_API FCommandData : public FTableRowBase
{
    GENERATED_BODY()

    // The command's unique identifier
    UPROPERTY(EditDefaultsOnly, Category = "Command")
    FGameplayTag CommandTag;

    // Tags required on the unit to execute this command
    UPROPERTY(EditDefaultsOnly, Category = "Command")
    FGameplayTagContainer RequiredUnitTags;

    // Cursor to display when this command is available
    UPROPERTY(EditDefaultsOnly, Category = "Command|UI")
    TSoftObjectPtr<UTexture2D> CursorTexture;

    // The gameplay tag that will trigger the ability
    UPROPERTY(EditDefaultsOnly, Category = "Command")
    FGameplayTag AbilityTriggerTag;
};

USTRUCT(BlueprintType)
struct FIRSTLINE_API FCommandEventData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FVector TargetLocation;

    UPROPERTY(BlueprintReadWrite)
    TArray<AActor*> SelectedUnits;

    UPROPERTY(BlueprintReadWrite)
    AActor* TargetActor;
};

UCLASS(BlueprintType)
class FIRSTLINE_API UCommandEventDataWrapper : public UObject
{
    GENERATED_BODY()
    
public:
    UPROPERTY(BlueprintReadWrite)
    FCommandEventData EventData;
}; 