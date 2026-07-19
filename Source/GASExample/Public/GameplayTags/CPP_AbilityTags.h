// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace CPPAbilityTags
{
	namespace Ability
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Primary);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Secondary);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tertiary);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActivateOnGiven);
	}
	namespace Event {
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(KillPoint);
		namespace Enemy {
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReact);
		}
	}
}

