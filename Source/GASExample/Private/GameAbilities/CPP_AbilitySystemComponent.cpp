// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilities/CPP_AbilitySystemComponent.h"
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
