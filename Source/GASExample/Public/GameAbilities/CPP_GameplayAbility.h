// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CPP_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GASEXAMPLE_API UCPP_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crash|Debug|Panel")
	bool bDrawDebug = false;
	
};
