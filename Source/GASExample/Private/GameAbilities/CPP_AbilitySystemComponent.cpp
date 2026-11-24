// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilities/CPP_AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTags/CPP_AbilityTags.h"

void UCPP_AbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);
	HandleAutoActivateAbilities(AbilitySpec);
}

void UCPP_AbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	FScopedAbilityListLock ActiveSpecLook(*this);
	for (const auto& AbilityInfo : GetActivatableAbilities())
	{
		HandleAutoActivateAbilities(AbilityInfo);
	}
}

bool UCPP_AbilitySystemComponent::TryActivateAbilityOrBufferCombo(const FGameplayTag& AbilityTag)
{
	if (!AbilityTag.IsValid())
	{
		return false;
	}

	if (TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer()))
	{
		SetLooseGameplayTagCount(CPPAbilityTags::State::Combo::InputBuffered, 0);
		return true;
	}

	if (!HasMatchingGameplayTag(CPPAbilityTags::State::Combo::Active))
	{
		return false;
	}

	AActor* Avatar = GetAvatarActor();
	if (!IsValid(Avatar))
	{
		return false;
	}

	SetLooseGameplayTagCount(CPPAbilityTags::State::Combo::InputBuffered, 1);

	FGameplayEventData Payload;
	Payload.EventTag = CPPAbilityTags::Event::Combo::InputBuffered;
	Payload.Instigator = Avatar;
	Payload.Target = Avatar;
	Payload.InstigatorTags.AddTag(AbilityTag);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		Avatar, CPPAbilityTags::Event::Combo::InputBuffered, Payload);
	return true;
}

void UCPP_AbilitySystemComponent::SetAbilityLevel(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level)
{
	if (IsValid(GetAvatarActor())&& !GetAvatarActor()->HasAuthority()) return;
	if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromClass(AbilityClass))
	{
		AbilitySpec->Level = Level;
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UCPP_AbilitySystemComponent::AddToAbilityLevel(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level)
{	if (IsValid(GetAvatarActor())&& !GetAvatarActor()->HasAuthority()) return;
	if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromClass(AbilityClass))
	{
		AbilitySpec->Level += Level;
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UCPP_AbilitySystemComponent::HandleAutoActivateAbilities(const FGameplayAbilitySpec& AbilitySpec)
{
	if (!IsValid(AbilitySpec.Ability)) return;

	auto TagContainer = AbilitySpec.Ability->GetAssetTags();
	for (const FGameplayTag& Tag : TagContainer) {
		if (Tag.MatchesTag(CPPAbilityTags::Ability::ActivateOnGiven)) {
			TryActivateAbility(AbilitySpec.Handle);
			return;
		}
	}
}
