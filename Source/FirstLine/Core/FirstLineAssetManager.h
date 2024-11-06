

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "FirstLineAssetManager.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class FIRSTLINE_API UFirstLineAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	UFirstLineAssetManager();

	virtual void StartInitialLoading() override;

	static UFirstLineAssetManager& Get();
};
