#include "CommandSystemComponent.h"
#include "Units/ICommandable.h"
#include "Commands/ICommandTarget.h"
#include "Core/AbilitySystem/FirstLineGameplayTags.h"

UCommandSystemComponent::UCommandSystemComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    bIsCacheValid = false;
}
void UCommandSystemComponent::BeginPlay()
{
    Super::BeginPlay();
    // Only needed on owning clients
    SetIsReplicated(false);
} 


const FCommandData UCommandSystemComponent::GetAvailableCommand(const TArray<AActor*>& SelectedUnits, AActor* HoveredActor)
{
    // Early return if nothing changed (cache hit)
    if (LastHoveredActor == HoveredActor && bIsCacheValid)
    {
        return CachedCommand;
    }

    // Reset cache state
    bIsCacheValid = false;
    LastHoveredActor = HoveredActor;

    // Validate input
    if (SelectedUnits.Num() == 0 || !CommandDataTable || !HoveredActor)
    {
        UE_LOG(LogTemp, Verbose, TEXT("Command System: Invalid input - Units: %d, DataTable: %d, Actor: %d"), 
            SelectedUnits.Num(), IsValid(CommandDataTable), IsValid(HoveredActor));
        return FCommandData();
    }

    // Calculate shared unit tags
    FGameplayTagContainer SharedUnitTags;
    bool bFirstUnit = true;
    for (AActor* Unit : SelectedUnits)
    {
        if (!Unit || !Unit->Implements<UCommandable>())
        {
            UE_LOG(LogTemp, Warning, TEXT("Command System: Invalid unit or missing ICommandable interface"));
            return FCommandData();
        }

        const FGameplayTagContainer& UnitTags = ICommandable::Execute_GetUnitDefinitionTags(Unit);
        if (bFirstUnit)
        {
            SharedUnitTags = UnitTags;
            bFirstUnit = false;
            UE_LOG(LogTemp, Verbose, TEXT("Command System: Initial unit tags: %s"), *UnitTags.ToString());
        }
        else
        {
            // Get intersection using FGameplayTagContainer's built-in functionality
            SharedUnitTags = SharedUnitTags.Filter(UnitTags);
            UE_LOG(LogTemp, Verbose, TEXT("Command System: Updated shared tags: %s"), *SharedUnitTags.ToString());
        }
    }

    // Get available command tags from target
    FGameplayTagContainer CommandTags;
    if (HoveredActor->Implements<UCommandTarget>())
    {
        CommandTags = ICommandTarget::Execute_GetAvailableCommandTags(HoveredActor);
        UE_LOG(LogTemp, Verbose, TEXT("Command System: Target %s provides commands: %s"), 
            *HoveredActor->GetName(), *CommandTags.ToString());
    }
    else
    {
        // Default to move command for non-interactive actors/ground
        CommandTags.AddTag(FFirstLineGameplayTags::Get().Command);
        UE_LOG(LogTemp, Verbose, TEXT("Command System: Using default move command for %s"), 
            *HoveredActor->GetName());
    }

    // Find first valid command
    for (const FGameplayTag& CommandTag : CommandTags)
    {
        const FCommandData* CommandData = CommandDataTable->FindRow<FCommandData>(CommandTag.GetTagName(), TEXT(""));
        if (!CommandData)
        {
            UE_LOG(LogTemp, Warning, TEXT("Command System: No command data found for tag: %s"), 
                *CommandTag.ToString());
            continue;
        }

        if (SharedUnitTags.HasAll(CommandData->RequiredUnitTags))
        {
            UE_LOG(LogTemp, Log, TEXT("Command System: Found valid command: %s"), 
                *CommandTag.ToString());
            CachedCommand = *CommandData;
            bIsCacheValid = true;
            return CachedCommand;
        }
        else
        {
            UE_LOG(LogTemp, Verbose, TEXT("Command System: Units missing required tags for command: %s"), 
                *CommandTag.ToString());
        }
    }

    UE_LOG(LogTemp, Verbose, TEXT("Command System: No valid command found for target %s"), 
        *HoveredActor->GetName());
    return FCommandData();
}

