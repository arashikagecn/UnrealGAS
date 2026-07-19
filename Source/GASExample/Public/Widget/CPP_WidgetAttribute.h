// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attributes/Cpp_AttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "CPP_WidgetAttribute.generated.h"

/**
 * 
 */
class UCPP_AttributeSet;
UCLASS()
class GASEXAMPLE_API UCPP_WidgetAttribute : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Crash|Widget")
	FGameplayAttribute Attribute;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Crash|Widget")
	FGameplayAttribute MaxAttribute;
	bool MatchAttribute(const TTuple<FGameplayAttribute,FGameplayAttribute>& AttributePair) const;
	void OnAttributeChanged(const TTuple<FGameplayAttribute,FGameplayAttribute>& AttributePair, UCPP_AttributeSet* AttributeSet);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Attribute Changed"))
	void BP_OnAttributeChanged(float NewValue, float NewMaxValue);
};
