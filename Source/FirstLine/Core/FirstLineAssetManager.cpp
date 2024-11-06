#include "FirstLineAssetManager.h"

#include "AbilitySystem/FirstLineGameplayTags.h"

UFirstLineAssetManager::UFirstLineAssetManager()
{
	UE_LOG(LogTemp, Warning, TEXT("FirstLineAssetManager Constructor Called"));

	if (GEngine && GEngine->AssetManager == this)
	{
		UE_LOG(LogTemp, Warning, TEXT("This is the active Asset Manager"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("This is NOT the active Asset Manager"));
	}
}

void UFirstLineAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UE_LOG(LogTemp, Warning, TEXT("Initializing UFirstLineAssetManager"));
	FFirstLineGameplayTags::InitializeNativeTags();
}

UFirstLineAssetManager& UFirstLineAssetManager::Get()
{
	check(GEngine);
    
	if (UFirstLineAssetManager* Singleton = Cast<UFirstLineAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in GEngine, make sure DefaultGame.ini is set to use FL_AssetManager"));
	return *NewObject<UFirstLineAssetManager>();
}
