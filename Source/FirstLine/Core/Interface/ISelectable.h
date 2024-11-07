// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISelectable.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UISelectable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FIRSTLINE_API IISelectable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Selection")
	void OnSelected();
    
	UFUNCTION(BlueprintNativeEvent, Category = "Selection")
	void OnDeselected();

	UFUNCTION(BlueprintNativeEvent, Category = "Selection")
	bool CanBeSelected() const;
};
