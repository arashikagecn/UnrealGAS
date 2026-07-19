// Fill out your copyright notice in the Description page of Project Settings.
#include "Player/CPP_PlayerState.h"
#include "AbilitySystemComponent.h"
#include "GameAbilities/CPP_AbilitySystemComponent.h"
#include "Attributes/CPP_AttributeSet.h"
ACPP_PlayerState::ACPP_PlayerState()
{
	SetNetUpdateFrequency(100.f);
	AbilitySystemComponent = CreateDefaultSubobject<UCPP_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UCPP_AttributeSet>("AttributeSet");
}

UAbilitySystemComponent* ACPP_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
