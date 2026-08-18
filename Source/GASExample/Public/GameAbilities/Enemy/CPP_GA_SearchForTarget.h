// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseCharacter.h"
#include "GameAbilities/CPP_GameplayAbility.h"
#include "Navigation/PathFollowingComponent.h"
#include "CPP_GA_SearchForTarget.generated.h"

class UAITask_MoveTo;
class ACPP_BaseCharacter;
class UAbilityTask_WaitDelay;
class AAIController;
class ACPP_EnemyCharacter;
class UCPP_AbilityAsync_WaitGameplayEvent;
/**
 * 
 */
UCLASS()
class GASEXAMPLE_API UCPP_GA_SearchForTarget : public UCPP_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UCPP_GA_SearchForTarget();
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName TagToFind = CPPTags::Player;
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<ACPP_EnemyCharacter> OwningEnemy;
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AAIController> EnemyAIController;
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<ACPP_BaseCharacter> TargetCharacter;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
private:
	UFUNCTION(BlueprintCallable)
	void StartSearch();
	UFUNCTION()
	void OnGameplayEventReceived( FGameplayEventData EventData);
	UPROPERTY()
	TObjectPtr<UCPP_AbilityAsync_WaitGameplayEvent> WaitGameplayEventTask;
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> WaitDelayTask;
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> AttackDelayTask;
	UFUNCTION()
	void OnAttackDelayFinished();
	bool Search();
	UFUNCTION(BlueprintCallable)
	void MoveAndAttack();
	UFUNCTION(BlueprintCallable)
	void AttackTarget(TEnumAsByte<EPathFollowingResult::Type> Result, AAIController* AIController);
	UPROPERTY()
	TObjectPtr<UAITask_MoveTo> MoveToTask;
	UFUNCTION()
	void DoAttack();
};
