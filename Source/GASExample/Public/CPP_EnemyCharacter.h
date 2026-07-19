// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseCharacter.h"
#include "CPP_EnemyCharacter.generated.h"

/**
 * 
 */
class UAbilitySystemComponent;
class UAttributeSet;
UCLASS()
class GASEXAMPLE_API ACPP_EnemyCharacter : public ACPP_BaseCharacter
{
	GENERATED_BODY()
	
public:
	ACPP_EnemyCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributesSet() const override { return AttributeSet; }
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, Category = "Crash|Abilities")
	TObjectPtr<UAttributeSet> AttributeSet;
};
