// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTags/CPP_AbilityTags.h"

namespace CPPAbilityTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, "CPPAbilityTags.None");

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
		UE_DEFINE_GAMEPLAY_TAG(Combo, "CPPAbilityTags.Ability.Combo");
		namespace ComboStages
		{
			UE_DEFINE_GAMEPLAY_TAG(Stage1, "CPPAbilityTags.Ability.Combo.Stage.1");
			UE_DEFINE_GAMEPLAY_TAG(Stage2, "CPPAbilityTags.Ability.Combo.Stage.2");
			UE_DEFINE_GAMEPLAY_TAG(Stage3, "CPPAbilityTags.Ability.Combo.Stage.3");
			UE_DEFINE_GAMEPLAY_TAG(Stage4, "CPPAbilityTags.Ability.Combo.Stage.4");
		}
		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG(Attack, "CPPAbilityTags.Ability.Enemy.Attack");
		}
	}

	namespace State
	{
		UE_DEFINE_GAMEPLAY_TAG(Death, "CPPAbilityTags.State.Death");
		namespace Combo
		{
			UE_DEFINE_GAMEPLAY_TAG(Active, "CPPAbilityTags.State.Combo.Active");
			UE_DEFINE_GAMEPLAY_TAG(Window, "CPPAbilityTags.State.Combo.Window");
			UE_DEFINE_GAMEPLAY_TAG(InputBuffered, "CPPAbilityTags.State.Combo.InputBuffered");
			UE_DEFINE_GAMEPLAY_TAG(Blocked, "CPPAbilityTags.State.Combo.Blocked");
			UE_DEFINE_GAMEPLAY_TAG(Stage1, "CPPAbilityTags.State.Combo.Stage.1");
			UE_DEFINE_GAMEPLAY_TAG(Stage2, "CPPAbilityTags.State.Combo.Stage.2");
			UE_DEFINE_GAMEPLAY_TAG(Stage3, "CPPAbilityTags.State.Combo.Stage.3");
			UE_DEFINE_GAMEPLAY_TAG(Stage4, "CPPAbilityTags.State.Combo.Stage.4");
		}
	}

	namespace Event {
		UE_DEFINE_GAMEPLAY_TAG(KillPoint, "CPPAbilityTags.Event.KillPoint");
		namespace Combo
		{
			UE_DEFINE_GAMEPLAY_TAG(WindowOpen, "CPPAbilityTags.Event.Combo.WindowOpen");
			UE_DEFINE_GAMEPLAY_TAG(WindowClose, "CPPAbilityTags.Event.Combo.WindowClose");
			UE_DEFINE_GAMEPLAY_TAG(InputBuffered, "CPPAbilityTags.Event.Combo.InputBuffered");
		}
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
