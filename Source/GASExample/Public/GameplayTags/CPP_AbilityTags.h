// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace CPPAbilityTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(None);

	namespace  SetByCaller
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Projectile);
	}
	namespace Ability
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Primary);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Secondary);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tertiary);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActivateOnGiven);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combo);
		namespace ComboStages
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stage1);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stage2);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stage3);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stage4);
		}
		namespace Enemy
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attack);
		}
	}
	namespace State
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Death);
		namespace Combo
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Active);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Window);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputBuffered);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Blocked);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stage1);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stage2);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stage3);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stage4);
		}
	}
	namespace Event {
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(KillPoint);
		namespace Combo
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(WindowOpen);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(WindowClose);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputBuffered);
		}
		namespace Player
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReact);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Death);
		}
		namespace Enemy {
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReact);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(FinishAttack);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(MeleeTraceHit);
		}
	}
}
