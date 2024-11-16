// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CollectableResource.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCollectableResource : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FIRSTLINE_API ICollectableResource
{
	GENERATED_BODY()	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FGameplayTag GetResourceTag() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int GetAvailableResource() const;

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetCollectionTime() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanCollect() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TakeResource(FGameplayTag& Resource, int& Amount);
	
};
