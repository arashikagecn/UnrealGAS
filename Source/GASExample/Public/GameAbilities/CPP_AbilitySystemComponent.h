// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CPP_AbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custon), meta = (BlueprintSpawnableComponent))
class GASEXAMPLE_API UCPP_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	virtual void OnRep_ActivateAbilities() override;
	UFUNCTION(BlueprintCallable, Category = "Crash|Attribute")
	void SetAbilityLevel(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level);
	UFUNCTION(BlueprintCallable, Category = "Crash|Attribute")
	void AddToAbilityLevel(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level = 1);
private:
	void HandleAutoActivateAbilities(const FGameplayAbilitySpec& AbilitySpec);
};
