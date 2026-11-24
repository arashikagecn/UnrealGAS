// Fill out your copyright notice in the Description page of Project Settings.

#include "GameAbilities/CPP_GA_ComboStage.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Animation/AnimMontage.h"
#include "GameAbilities/CPP_AbilitySystemComponent.h"
#include "GameplayTags/CPP_AbilityTags.h"

DEFINE_LOG_CATEGORY_STATIC(LogCPPCombo, Log, All);

UCPP_GA_ComboStage::UCPP_GA_ComboStage()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	FGameplayTagContainer ComboAssetTags;
	ComboAssetTags.AddTag(CPPAbilityTags::Ability::Combo);
	SetAssetTags(ComboAssetTags);

	ActivationBlockedTags.AddTag(CPPAbilityTags::State::Death);
	ActivationBlockedTags.AddTag(CPPAbilityTags::State::Combo::Blocked);
}

bool UCPP_GA_ComboStage::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (!IsValid(ComboMontage) || !ComboInputTag.IsValid() || !ComboStageTag.IsValid())
	{
		UE_LOG(LogCPPCombo, Error,
			TEXT("%s is missing ComboMontage, ComboInputTag, or ComboStageTag."), *GetNameSafe(this));
		return false;
	}

	if (!GetAssetTags().HasTag(ComboInputTag))
	{
		UE_LOG(LogCPPCombo, Error,
			TEXT("%s must include %s in its Asset Tags."),
			*GetNameSafe(this), *ComboInputTag.ToString());
		return false;
	}

	const UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
	if (!IsValid(ASC))
	{
		return false;
	}

	if (!RequiredPreviousStageTag.IsValid())
	{
		return !ASC->HasMatchingGameplayTag(CPPAbilityTags::State::Combo::Active);
	}

	return ASC->HasMatchingGameplayTag(CPPAbilityTags::State::Combo::Window)
		&& ASC->HasMatchingGameplayTag(RequiredPreviousStageTag);
}

void UCPP_GA_ComboStage::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		FinishAbility(true);
		return;
	}

	UCPP_AbilitySystemComponent* ASC = Cast<UCPP_AbilitySystemComponent>(
		ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr);
	if (!IsValid(ASC))
	{
		FinishAbility(true);
		return;
	}

	// Consume the transition window before the next candidate in the same
	// TryActivateAbilitiesByTag call can evaluate its requirements.
	ASC->SetLooseGameplayTagCount(CPPAbilityTags::State::Combo::Window, 0);
	ClearInputBuffer();

	ASC->AddLooseGameplayTag(CPPAbilityTags::State::Combo::Active);
	bAppliedActiveTag = true;
	ASC->AddLooseGameplayTag(ComboStageTag);
	bAppliedStageTag = true;

	WindowOpenTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, CPPAbilityTags::Event::Combo::WindowOpen);
	WindowCloseTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, CPPAbilityTags::Event::Combo::WindowClose);
	InputBufferedTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, CPPAbilityTags::Event::Combo::InputBuffered);

	if (!IsValid(WindowOpenTask) || !IsValid(WindowCloseTask) || !IsValid(InputBufferedTask))
	{
		FinishAbility(true);
		return;
	}

	WindowOpenTask->EventReceived.AddDynamic(this, &ThisClass::HandleWindowOpened);
	WindowCloseTask->EventReceived.AddDynamic(this, &ThisClass::HandleWindowClosed);
	InputBufferedTask->EventReceived.AddDynamic(this, &ThisClass::HandleInputBuffered);
	WindowOpenTask->ReadyForActivation();
	WindowCloseTask->ReadyForActivation();
	InputBufferedTask->ReadyForActivation();

	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		ComboMontage,
		PlayRate,
		StartSection,
		true,
		AnimRootMotionTranslationScale);
	if (!IsValid(MontageTask))
	{
		FinishAbility(true);
		return;
	}

	MontageTask->OnCompleted.AddDynamic(this, &ThisClass::HandleMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::HandleMontageInterrupted);
	MontageTask->OnCancelled.AddDynamic(this, &ThisClass::HandleMontageCancelled);
	MontageTask->ReadyForActivation();

	const FGameplayEventData EmptyEventData;
	OnComboStageStarted(TriggerEventData ? *TriggerEventData : EmptyEventData);
}

void UCPP_GA_ComboStage::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	CleanupComboState();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCPP_GA_ComboStage::HandleMontageCompleted()
{
	FinishAbility(false);
}

void UCPP_GA_ComboStage::HandleMontageInterrupted()
{
	FinishAbility(true);
}

void UCPP_GA_ComboStage::HandleMontageCancelled()
{
	FinishAbility(true);
}

void UCPP_GA_ComboStage::HandleWindowOpened(const FGameplayEventData Payload)
{
	UpdateWindowTag(true);
	OnComboWindowOpened();

	if (BufferedInputTag.IsValid() || HasComboState(CPPAbilityTags::State::Combo::InputBuffered))
	{
		TryStartNextStage();
	}
}

void UCPP_GA_ComboStage::HandleWindowClosed(const FGameplayEventData Payload)
{
	UpdateWindowTag(false);
	ClearInputBuffer();
	BufferedInputTag = FGameplayTag();
	OnComboWindowClosed();
}

void UCPP_GA_ComboStage::HandleInputBuffered(const FGameplayEventData Payload)
{
	for (const FGameplayTag& RequestedTag : Payload.InstigatorTags)
	{
		if (RequestedTag.IsValid())
		{
			BufferedInputTag = RequestedTag;
			break;
		}
	}

	if (!BufferedInputTag.IsValid())
	{
		BufferedInputTag = ComboInputTag;
	}

	OnComboInputBuffered(BufferedInputTag);
	if (HasComboState(CPPAbilityTags::State::Combo::Window))
	{
		TryStartNextStage();
	}
}

bool UCPP_GA_ComboStage::HasComboState(const FGameplayTag& Tag) const
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	return IsValid(ASC) && ASC->HasMatchingGameplayTag(Tag);
}

bool UCPP_GA_ComboStage::TryStartNextStage()
{
	if (!HasComboState(CPPAbilityTags::State::Combo::Window))
	{
		return false;
	}

	UCPP_AbilitySystemComponent* ASC = Cast<UCPP_AbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (!IsValid(ASC))
	{
		ClearInputBuffer();
		return false;
	}

	const FGameplayTag InputTag = BufferedInputTag.IsValid() ? BufferedInputTag : ComboInputTag;
	if (!InputTag.IsValid())
	{
		ClearInputBuffer();
		return false;
	}

	const bool bStartedNextStage = ASC->TryActivateAbilitiesByTag(InputTag.GetSingleTagContainer());
	ClearInputBuffer();
	BufferedInputTag = FGameplayTag();
	return bStartedNextStage;
}

void UCPP_GA_ComboStage::UpdateWindowTag(const bool bOpen)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!IsValid(ASC))
	{
		return;
	}

	if (bOpen && !bAppliedWindowTag)
	{
		ASC->AddLooseGameplayTag(CPPAbilityTags::State::Combo::Window);
		bAppliedWindowTag = true;
	}
	else if (!bOpen && bAppliedWindowTag)
	{
		ASC->RemoveLooseGameplayTag(CPPAbilityTags::State::Combo::Window);
		bAppliedWindowTag = false;
	}
}

void UCPP_GA_ComboStage::ClearInputBuffer()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->SetLooseGameplayTagCount(CPPAbilityTags::State::Combo::InputBuffered, 0);
	}
}

void UCPP_GA_ComboStage::CleanupComboState()
{
	if (bComboStateCleaned)
	{
		return;
	}
	bComboStateCleaned = true;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!IsValid(ASC))
	{
		return;
	}

	ClearInputBuffer();
	if (bAppliedWindowTag)
	{
		ASC->RemoveLooseGameplayTag(CPPAbilityTags::State::Combo::Window);
		bAppliedWindowTag = false;
	}
	if (bAppliedStageTag)
	{
		ASC->RemoveLooseGameplayTag(ComboStageTag);
		bAppliedStageTag = false;
	}
	if (bAppliedActiveTag)
	{
		ASC->RemoveLooseGameplayTag(CPPAbilityTags::State::Combo::Active);
		bAppliedActiveTag = false;
	}
}

void UCPP_GA_ComboStage::FinishAbility(const bool bWasCancelled)
{
	if (IsEndAbilityValid(CurrentSpecHandle, CurrentActorInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, bWasCancelled);
	}
}
