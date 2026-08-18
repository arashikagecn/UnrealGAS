// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_WidgetComponent.h"
#include "CPP_BaseCharacter.h"
#include "GameAbilities/CPP_AbilitySystemComponent.h"
#include "Attributes/CPP_AttributeSet.h"
#include "Blueprint/WidgetTree.h"
#include "Widget/CPP_WidgetAttribute.h"

void UCPP_WidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	InitAbilitySystemData();
	if (!IsASCInitialized()) {
		OwnerCharacter->OnASCInitialized.AddDynamic(this, &UCPP_WidgetComponent::OnASCInitialized);
		return;
	}
	InitializeAttributeDelegate();
}

void UCPP_WidgetComponent::InitAbilitySystemData()
{
	OwnerCharacter = Cast<ACPP_BaseCharacter>(GetOwner());
	AbilitySystemComponent = Cast<UCPP_AbilitySystemComponent>(OwnerCharacter->GetAbilitySystemComponent());
	AttributeSet = Cast<UCPP_AttributeSet>(OwnerCharacter->GetAttributesSet());
}

bool UCPP_WidgetComponent::IsASCInitialized() const
{
	return AbilitySystemComponent.IsValid() && AttributeSet.IsValid();
}

void UCPP_WidgetComponent::OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	AbilitySystemComponent = Cast<UCPP_AbilitySystemComponent>(ASC);
	AttributeSet = Cast<UCPP_AttributeSet>(AS);
	if (IsASCInitialized()) InitializeAttributeDelegate();
}

void UCPP_WidgetComponent::BindToAttributesChange()
{
	for (const auto& Pair : AttributeMap)
	{
		BindWidgetAttributeChanges(GetUserWidgetObject(), Pair);
		//Otherwise, loop over all children on owned widget, and do same for them
		GetUserWidgetObject()->WidgetTree->ForEachWidget([this, &Pair](UWidget* ChildWidget)
		{
			BindWidgetAttributeChanges(ChildWidget, Pair);
		});
	}
}

void UCPP_WidgetComponent::InitializeAttributeDelegate()
{
	if (!AttributeSet->bAttributesInitialized)
	{
		AttributeSet->OnAttributeInitialised.AddDynamic(this, &UCPP_WidgetComponent::BindToAttributesChange);
	}else
	{
		BindToAttributesChange();
	}
}

void UCPP_WidgetComponent::BindWidgetAttributeChanges(UWidget* WidgetObject,
	const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const
{

	UCPP_WidgetAttribute* WidgetAttribute = Cast<UCPP_WidgetAttribute>(WidgetObject);	//Check the Given object owned by this component, see if it is a UCPP_WidgetAttribute component, handle it if so
	if (!IsValid(WidgetAttribute)) return;
	if (!WidgetAttribute->MatchAttribute(Pair)) return;
	WidgetAttribute->AvatarActor = OwnerCharacter;
	WidgetAttribute->OnAttributeChanged(Pair, AttributeSet.Get(), 0.f);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Key).AddLambda([this, WidgetAttribute, &Pair](const FOnAttributeChangeData& AttributeChangeData)
	{
		WidgetAttribute->OnAttributeChanged(Pair, AttributeSet.Get(), AttributeChangeData.OldValue);
	});
}
