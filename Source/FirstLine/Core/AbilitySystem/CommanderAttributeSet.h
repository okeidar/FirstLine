// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CommanderAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class FIRSTLINE_API UCommanderAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UCommanderAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	ATTRIBUTE_ACCESSORS(UCommanderAttributeSet, CurrentPopulation)
    ATTRIBUTE_ACCESSORS(UCommanderAttributeSet, MaxPopulation)

protected:
	UFUNCTION()
	virtual void OnRep_CurrentPopulation(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxPopulation(const FGameplayAttributeData& OldValue);

private:
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_CurrentPopulation, meta=(AllowPrivateAccess=true))
	FGameplayAttributeData CurrentPopulation;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxPopulation,meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxPopulation;




};

