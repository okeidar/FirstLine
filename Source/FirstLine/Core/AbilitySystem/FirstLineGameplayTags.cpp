#include "FirstLineGameplayTags.h"
#include "GameplayTagsManager.h"

FFirstLineGameplayTags FFirstLineGameplayTags::GameplayTags;



void FFirstLineGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
    UE_LOG(LogTemp, Warning, TEXT("Initializing Native Gameplay Tags"));

	GameplayTags.Ability_Cooldown = Manager.AddNativeGameplayTag(
		FName("Ability.Cooldown"),
		FString("Ability is on cooldown")
	);

	GameplayTags.Ability_AutoRetrigger = Manager.AddNativeGameplayTag(
		FName("Ability.AutoRetrigger"),
		FString("Ability that automatically retriggers after cooldown")
	);

	Manager.DoneAddingNativeTags();
}
