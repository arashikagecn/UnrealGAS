// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameAbilities/CPP_GameplayAbility.h"
#include "CPP_GA_ComboStage.generated.h"

class UAnimMontage;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;
struct FGameplayEventData;
struct FGameplayTag;

/**
 * A single combo node. Each Blueprint child is a separate GameplayAbility with
 * its own montage and stage tags.
 *
 * Required setup:
 * - ComboInputTag is also present in the child's Asset Tags.
 * - ComboStageTag is unique for this stage.
 * - RequiredPreviousStageTag is empty only for the first stage.
 */
UCLASS(Abstract, Blueprintable)
class GASEXAMPLE_API UCPP_GA_ComboStage : public UCPP_GameplayAbility
{
	GENERATED_BODY()

public:
	UCPP_GA_ComboStage();

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

protected:
	/** Montage owned by this combo stage. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crash|Combo")
	TObjectPtr<UAnimMontage> ComboMontage;

	/** Input tag used to activate this ability. It must also be present in Asset Tags. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crash|Combo")
	FGameplayTag ComboInputTag;

	/** Unique activation-owned tag for this stage. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crash|Combo")
	FGameplayTag ComboStageTag;

	/** Leave empty for the first stage. Later stages require the preceding stage tag. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crash|Combo")
	FGameplayTag RequiredPreviousStageTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crash|Combo")
	float PlayRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crash|Combo")
	FName StartSection = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crash|Combo")
	float AnimRootMotionTranslationScale = 1.f;

	/** Called after this stage has consumed its incoming window and started. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Crash|Combo")
	void OnComboStageStarted(const FGameplayEventData& TriggerEventData);

	/** Called when the montage's combo-window notify state begins. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Crash|Combo")
	void OnComboWindowOpened();

	/** Called when the montage's combo-window notify state ends. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Crash|Combo")
	void OnComboWindowClosed();

	/** Called when an input arrives before the window can activate a next stage. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Crash|Combo")
	void OnComboInputBuffered(FGameplayTag RequestedInputTag);

private:
	UFUNCTION()
	void HandleMontageCompleted();

	UFUNCTION()
	void HandleMontageInterrupted();

	UFUNCTION()
	void HandleMontageCancelled();

	UFUNCTION()
	void HandleWindowOpened(const FGameplayEventData Payload);

	UFUNCTION()
	void HandleWindowClosed(const FGameplayEventData Payload);

	UFUNCTION()
	void HandleInputBuffered(const FGameplayEventData Payload);

	bool HasComboState(const FGameplayTag& Tag) const;
	bool TryStartNextStage();
	void UpdateWindowTag(bool bOpen);
	void ClearInputBuffer();
	void CleanupComboState();
	void FinishAbility(bool bWasCancelled);

	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WindowOpenTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WindowCloseTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> InputBufferedTask;

	FGameplayTag BufferedInputTag;
	bool bAppliedActiveTag = false;
	bool bAppliedStageTag = false;
	bool bAppliedWindowTag = false;
	bool bComboStateCleaned = false;
};
