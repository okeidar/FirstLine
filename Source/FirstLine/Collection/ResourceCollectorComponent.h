// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableResource.h"
#include "Components/ActorComponent.h"
#include "ResourceCollectorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FIRSTLINE_API UResourceCollectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UResourceCollectorComponent();

	UFUNCTION(BlueprintCallable)
	bool TakeResourceFrom(const TScriptInterface<ICollectableResource>& ResourceOwner);
	
	UFUNCTION(BlueprintCallable)
	void DeliverResources(FGameplayTag& ResourceTag, int& Amount);

protected:

	UPROPERTY(EditAnywhere)
	bool bHoldingResource = false;

	
	UPROPERTY(EditAnywhere)
	FGameplayTag CurrentlyHeldResource;
	
	UPROPERTY(EditAnywhere)
	int CurrentResourceAmount = 0 ;
};
