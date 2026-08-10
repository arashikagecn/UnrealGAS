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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributesSet() const override { return AttributeSet; }
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Crash|AI")
	float AcceptanceRadius {500.0f};
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Crash|AI")
	float MinAttackDelay {0.1f};
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Crash|AI")
	float MaxAttackDelay {0.5f};
	UFUNCTION(BlueprintImplementableEvent)
	float GetTimelineLength();
	UFUNCTION(BlueprintCallable, Category="Crash|AI")
	void StopMovementUntilLanded();
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	bool bIsBeingLaunched{false};
protected:
	virtual void BeginPlay() override;
	virtual void HandleDeath() override;
private:
	UFUNCTION()
	void EnableMovementWhenLanded(const FHitResult& Hit);
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, Category = "Crash|Abilities")
	TObjectPtr<UAttributeSet> AttributeSet;
};
