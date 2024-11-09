#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "ICommandable.generated.h"

/**
 * Interface for units that can receive and execute commands
 * Provides unit capabilities and command execution
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UCommandable : public UInterface
{
    GENERATED_BODY()
};

class FIRSTLINE_API ICommandable
{
    GENERATED_BODY()

public:
    /** 
     * Execute a command on this unit
     * @param CommandTag - The command to execute
     * @param Location - Target location for the command
     * @param Target - Optional target actor for the command
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Command")
    void ExecuteCommand(const FGameplayTag& CommandTag, const FVector& Location, AActor* Target);

    /** 
     * Get this unit's capability tags
     * Used to validate available commands
     * @return Container of unit capability tags
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Command")
    const FGameplayTagContainer GetUnitDefinitionTags() const;
}; 