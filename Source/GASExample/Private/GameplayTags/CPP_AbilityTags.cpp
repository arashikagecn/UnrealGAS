// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTags/CPP_AbilityTags.h"

namespace CPPAbilityTags
{
	namespace Ability
	{
		UE_DEFINE_GAMEPLAY_TAG(Primary, "CPPAbilityTags.Ability.Primary");
		UE_DEFINE_GAMEPLAY_TAG(Secondary, "CPPAbilityTags.Ability.Secondary");
		UE_DEFINE_GAMEPLAY_TAG(Tertiary, "CPPAbilityTags.Ability.Tertiary");
		UE_DEFINE_GAMEPLAY_TAG(ActivateOnGiven, "CPPAbilityTags.Ability.ActivateOnGiven");

	}

	namespace Event {
		UE_DEFINE_GAMEPLAY_TAG(KillPoint, "CPPAbilityTags.Event.KillPoint");
		namespace Enemy {
			UE_DEFINE_GAMEPLAY_TAG(HitReact, "CPPAbilityTags.Event.Enemy.HitReact");
		}
	}
}