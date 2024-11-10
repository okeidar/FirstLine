#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CommandTypes.h"
#include "CommandSystemComponent.generated.h"

/**
 * Handles command selection and validation for RTS-style unit commands
 * Caches results to prevent unnecessary checks every frame
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FIRSTLINE_API UCommandSystemComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCommandSystemComponent();

    /** 
     * Returns the available command based on selected units and hovered target
     * Caches result until target changes
     * @param SelectedUnits - Array of currently selected units
     * @param HoveredActor - Actor currently under cursor
     * @return Command data if valid command found, nullptr otherwise
     */
    UFUNCTION(BlueprintCallable, Category = "Command")
    const FCommandData GetAvailableCommand(const TArray<AActor*>& SelectedUnits, AActor* HoveredActor, const FVector& Location);

protected:
    /** Data table containing all possible commands mapped by command tags */
    UPROPERTY(EditDefaultsOnly, Category = "Command")
    UDataTable* CommandDataTable;

    virtual void BeginPlay() override;
private:
    // Cached data to prevent unnecessary updates
    UPROPERTY()
    TWeakObjectPtr<AActor> LastHoveredActor;
    
    UPROPERTY()
    FCommandData CachedCommand;
    
    bool bIsCacheValid;
}; 