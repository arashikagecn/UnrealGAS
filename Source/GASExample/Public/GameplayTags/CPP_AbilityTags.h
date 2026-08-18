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
		namespace Enemy
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attack);
		}
	}
	namespace Event {
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(KillPoint);
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
