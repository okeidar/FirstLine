#include "FirstLineGameplayTags.h"
#include "GameplayTagsManager.h"

FFirstLineGameplayTags FFirstLineGameplayTags::GameplayTags;



void FFirstLineGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
    UE_LOG(LogTemp, Warning, TEXT("Initializing Native Gameplay Tags"));

	// Initialize root tags for commands and unit capabilities
	GameplayTags.Command = Manager.AddNativeGameplayTag(
		FName("Command"),
		FString("Root tag for all unit commands")
	);

	GameplayTags.Unit = Manager.AddNativeGameplayTag(
		FName("Unit"),
		FString("Root tag for unit capabilities")
	);

	GameplayTags.Ability_Cooldown = Manager.AddNativeGameplayTag(
		FName("Ability.Cooldown"),
		FString("Ability is on cooldown")
	);

	GameplayTags.Ability_AutoRetrigger = Manager.AddNativeGameplayTag(
		FName("Ability.AutoRetrigger"),
		FString("Ability that automatically retriggers after cooldown")
	);

	GameplayTags.Command_Move = Manager.AddNativeGameplayTag(
		FName("Command.Move"),
		FString("Command to move units to location")
	);

	Manager.DoneAddingNativeTags();
}
