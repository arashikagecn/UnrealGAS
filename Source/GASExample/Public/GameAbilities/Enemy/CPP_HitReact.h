// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameAbilities/CPP_GameplayAbility.h"
#include "CPP_HitReact.generated.h"

/**
 * 
 */
UCLASS()
class GASEXAMPLE_API UCPP_HitReact : public UCPP_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Crash|Abilities")
	FVector AvatarForward;
	UPROPERTY(BlueprintReadOnly, Category = "Crash|Abilities")
	FVector ToInstigator;
	UFUNCTION(BlueprintCallable, Category = "Crash|Abilities")
	void CacheHitDirectionVectors(AActor* Instigator);
};
