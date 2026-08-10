// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTags/CPP_AbilityTags.h"

namespace CPPAbilityTags
{
	namespace  SetByCaller
	{
		UE_DEFINE_GAMEPLAY_TAG(Projectile, "CPPAbilityTags.SetByCaller.Projectile");
	}
	namespace Ability
	{
		UE_DEFINE_GAMEPLAY_TAG(Primary, "CPPAbilityTags.Ability.Primary");
		UE_DEFINE_GAMEPLAY_TAG(Secondary, "CPPAbilityTags.Ability.Secondary");
		UE_DEFINE_GAMEPLAY_TAG(Tertiary, "CPPAbilityTags.Ability.Tertiary");
		UE_DEFINE_GAMEPLAY_TAG(ActivateOnGiven, "CPPAbilityTags.Ability.ActivateOnGiven");
		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG(Attack, "CPPAbilityTags.Ability.Enemy.Attack");
		}
	}

	namespace Event {
		UE_DEFINE_GAMEPLAY_TAG(KillPoint, "CPPAbilityTags.Event.KillPoint");
		namespace Player
		{
			UE_DEFINE_GAMEPLAY_TAG(HitReact, "CPPAbilityTags.Event.Player.HitReact");
			UE_DEFINE_GAMEPLAY_TAG(Death, "CPPAbilityTags.Event.Player.Death");
		}
		namespace Enemy {
			UE_DEFINE_GAMEPLAY_TAG(HitReact, "CPPAbilityTags.Event.Enemy.HitReact");
			UE_DEFINE_GAMEPLAY_TAG(FinishAttack, "CPPAbilityTags.Event.Enemy.FinishAttack");
			UE_DEFINE_GAMEPLAY_TAG(MeleeTraceHit, "CPPAbilityTags.Event.Enemy.MeleeTraceHit");
		}
	}
}