

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "FirstLineGameplayTags.generated.h"

class UGameplayTagsManager;

USTRUCT(Blueprintable)
struct FFirstLineGameplayTags
{
	GENERATED_BODY()
public:
	static const FFirstLineGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeTags();

	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag Ability_Cooldown;
    
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag Ability_AutoRetrigger;

protected:
	static FFirstLineGameplayTags GameplayTags;
};

UCLASS()
class FIRSTLINE_API UFirstLinTagsBlueprintLibrary: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static const FFirstLineGameplayTags& GetGameplayTags()  { return FFirstLineGameplayTags::Get(); }
};