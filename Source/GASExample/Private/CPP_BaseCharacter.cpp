// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

namespace CPPTags
{
	 const FName Player = FName("Player");
}


// Sets default values
ACPP_BaseCharacter::ACPP_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Tick and refresh bones even when not rendered, so that the bone state is always up to date
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

void ACPP_BaseCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

UAbilitySystemComponent* ACPP_BaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}

void ACPP_BaseCharacter::GiveStartupAbilities()
{
	for (const auto& Ability : StartUpAbilities) {
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
		GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
	}
}

void ACPP_BaseCharacter::InitializeAttributes() const
{
	checkf(IsValid(InitialAttributesEffect), TEXT("InitialAttributesEffect is NOT ready"));
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	auto SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(InitialAttributesEffect, 1, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ACPP_BaseCharacter::OnHealthChanged(const FOnAttributeChangeData& AttributeChangeData)
{
	if (AttributeChangeData.NewValue <= 0.f) HandleDeath();
}

void ACPP_BaseCharacter::HandleDeath()
{
	bAlive = false;
	
}

void ACPP_BaseCharacter::HandleRespawn()
{
	bAlive = true;
}

void ACPP_BaseCharacter::ResetAttributes()
{
	checkf(ResetAttributesEffect, TEXT("ResetAttributesEffect is NOT ready"));
	if (!IsValid(GetAbilitySystemComponent())) return;
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(ResetAttributesEffect, 1, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}
