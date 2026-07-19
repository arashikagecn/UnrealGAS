// Fill out your copyright notice in the Description page of Project Settings.


#include "Attributes/CPP_AttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameplayTags/CPP_AbilityTags.h"
#include "Net/UnrealNetwork.h"

void UCPP_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldValue);
}

void UCPP_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, OldValue);
}

void UCPP_AttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Mana, OldValue);
}

void UCPP_AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxMana, OldValue);
}

void UCPP_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UCPP_AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCPP_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCPP_AttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCPP_AttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME(UCPP_AttributeSet, bAttributesInitialized);
}
 
void UCPP_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetHealthAttribute() && GetHealth()<= 0.f)
	{
		FGameplayEventData Payload;
		Payload.Instigator = Data.Target.GetAvatarActor();
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Data.EffectSpec.GetContext().GetInstigator(),CPPAbilityTags::Event::KillPoint, Payload);
		
	}
	if (!bAttributesInitialized)
	{
		bAttributesInitialized = true;
		OnAttributeInitialised.Broadcast();
	}
}

void UCPP_AttributeSet::OnRep_AttributesInitialized()
{
	if (!bAttributesInitialized) OnAttributeInitialised.Broadcast();
}
