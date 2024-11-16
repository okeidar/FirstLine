// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceCollectorComponent.h"


// Sets default values for this component's properties
UResourceCollectorComponent::UResourceCollectorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}



bool UResourceCollectorComponent::TakeResourceFrom(const TScriptInterface<ICollectableResource>& ResourceOwner)
{
	if(bHoldingResource)
	{
		//TODO: log
		return false;
	}
	if(!ResourceOwner.GetObject())
	{
		//TODO: log
		return false;
	}
	if(!ICollectableResource::Execute_CanCollect(ResourceOwner.GetObject()))
	{
		//TODO: log
		return false;
	}
	ICollectableResource::Execute_TakeResource(ResourceOwner.GetObject(),CurrentlyHeldResource,CurrentResourceAmount);
	bHoldingResource=true;
	return true;
}

void UResourceCollectorComponent::DeliverResources(FGameplayTag& ResourceTag, int& Amount)
{
	if(!bHoldingResource)
	{
		//TODO: log
		ResourceTag=FGameplayTag();
		Amount=0;
		return;
	}

	ResourceTag=CurrentlyHeldResource;
	Amount=CurrentResourceAmount;
	
	bHoldingResource=false;
	CurrentlyHeldResource=FGameplayTag();
	CurrentResourceAmount=0;
}

