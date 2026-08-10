// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameAbilities/CPP_GameplayAbility.h"
#include "CPP_GA_Primary.generated.h"

/**
 * 
 */
UCLASS()
class GASEXAMPLE_API UCPP_GA_Primary : public UCPP_GameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Crash|Abilities")
	void SendHitReactEventToActors(const TArray<AActor*>& HitActors);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crash|Abilities")
	float HitBoxRadius = 100.f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Crash|Abilities")
	float HitBoxForwardOffset = 100.f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Crash|Abilities")
	float HitBoxEvalationOffset = 20.f;
};
