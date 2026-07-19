// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilities/CPP_GameplayAbility.h"

void UCPP_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (bDrawDebug && IsValid(GEngine)) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("GameplayAbility::Activate called for ability: %s"), *GetName()));
	}
}
