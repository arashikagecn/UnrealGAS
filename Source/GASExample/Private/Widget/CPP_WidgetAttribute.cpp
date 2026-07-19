// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_WidgetAttribute.h"
#include "Attributes/CPP_AttributeSet.h"
bool UCPP_WidgetAttribute::MatchAttribute(const TTuple<FGameplayAttribute, FGameplayAttribute>& AttributePair) const
{
	return Attribute == AttributePair.Key && MaxAttribute == AttributePair.Value;
}

void UCPP_WidgetAttribute::OnAttributeChanged(const TTuple<FGameplayAttribute, FGameplayAttribute>& AttributePair,
	UCPP_AttributeSet* AttributeSet)
{
	const auto AttributeValue = AttributePair.Key.GetNumericValue(AttributeSet);
	const auto MaxAttributeValue = AttributePair.Value.GetNumericValue(AttributeSet);
	BP_OnAttributeChanged(AttributeValue, MaxAttributeValue);
}
