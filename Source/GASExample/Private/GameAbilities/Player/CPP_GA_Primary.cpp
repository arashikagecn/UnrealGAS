// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilities/Player/CPP_GA_Primary.h"
#include "Engine/OverlapResult.h"
#include "GameplayTags/CPP_AbilityTags.h"
#include "AbilitySystemBlueprintLibrary.h"

void UCPP_GA_Primary::SendHitReactEventToActors(const TArray<AActor*>& HitActors)
{
	for (const auto& HitActor : HitActors) {
		FGameplayEventData Payload;
		Payload.Instigator = GetAvatarActorFromActorInfo();
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, CPPAbilityTags::Event::Enemy::HitReact, Payload);
	}
}