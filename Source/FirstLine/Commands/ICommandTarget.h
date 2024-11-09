#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "ICommandTarget.generated.h"

/**
 * Interface for actors that can be targeted by unit commands
 * Provides available commands based on actor type and state
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UCommandTarget : public UInterface
{
    GENERATED_BODY()
};

class FIRSTLINE_API ICommandTarget
{
    GENERATED_BODY()

public:
    /** 
     * Returns the available command tags for this target
     * Command tags are used to look up command data in the data table
     * @return Container of available command tags
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Command")
    FGameplayTagContainer GetAvailableCommandTags() const;
}; 