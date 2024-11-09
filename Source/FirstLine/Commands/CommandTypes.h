#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
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

    // The ability to grant to the player
    UPROPERTY(EditDefaultsOnly, Category = "Command")
    TSubclassOf<UGameplayAbility> CommandAbility;
}; 