// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourceTagToAttributeConversionExecution.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "AttributeSet.h"
#include "NativeGameplayTags.h"

#include "AbilitySystem/CommanderAttributeSet.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(PAYLOAD_TAG, "SetByCaller.Payload");

UResourceTagToAttributeConversionExecution::UResourceTagToAttributeConversionExecution()
{
    // We need to define FAggregatorEvaluateParameters before Execute_Implementation
    FAggregatorEvaluateParameters EvaluateParameters;

    // Get all properties from the CommanderAttributeSet
    for (TFieldIterator<FProperty> It(UCommanderAttributeSet::StaticClass()); It; ++It)
    {
        if (FGameplayAttribute::IsGameplayAttributeDataProperty(*It))
        {
            // Add a capture definition for each attribute
            RelevantAttributesToCapture.Add(
                FGameplayEffectAttributeCaptureDefinition(
                    FGameplayAttribute(*It),
                    EGameplayEffectAttributeCaptureSource::Target,
                    true
                )
            );
        }
    }
}

void UResourceTagToAttributeConversionExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    float PayloadValue = Spec.GetSetByCallerMagnitude(PAYLOAD_TAG);
    UE_LOG(LogTemp, Log, TEXT("Execute_Implementation - Payload Value: %f"), PayloadValue);

    const FGameplayTagContainer& DynamicTags = Spec.GetDynamicAssetTags();
    
    if (DynamicTags.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No dynamic asset tags provided for resource conversion."));
        return;
    }

    // Get the target's ASC
    UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
    if (!TargetASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("Target ASC not found"));
        return;
    }

    FAggregatorEvaluateParameters EvaluateParams;
    EvaluateParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    EvaluateParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FString Prefix = "Resource.";    
    
    for (const FGameplayTag& ResourceTag : DynamicTags)
    {
        FString TagString = ResourceTag.ToString();

        if (!TagString.StartsWith(Prefix))
        {
            continue;
        }

        // Convert tag to attribute name (e.g., "Resource.Wood" -> "Wood")
        FString AttributeName = TagString.Mid(Prefix.Len());
        
        // Find the attribute property by name
        FProperty* Property = FindFProperty<FProperty>(UCommanderAttributeSet::StaticClass(), *AttributeName);
        if (FGameplayAttribute::IsGameplayAttributeDataProperty(Property))
        {
            FGameplayAttribute Attribute(Property);
            if (Attribute.IsValid())
            {
                float CurrentValue = 0.0f;
                ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
                    FGameplayEffectAttributeCaptureDefinition(Attribute, EGameplayEffectAttributeCaptureSource::Target, true),
                    EvaluateParams, CurrentValue);

                UE_LOG(LogTemp, Log, TEXT("Current value of %s before modification: %f"), 
                    *AttributeName, CurrentValue);

                OutExecutionOutput.AddOutputModifier(
                    FGameplayModifierEvaluatedData(Attribute, EGameplayModOp::Additive, PayloadValue));
                
                UE_LOG(LogTemp, Log, TEXT("Added modifier - Attribute: %s, Value to add: %f"), 
                    *Attribute.GetName(), PayloadValue);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Invalid gameplay attribute for property: %s"), *AttributeName);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Property is not a gameplay attribute: %s"), *AttributeName);
        }
    }
}
