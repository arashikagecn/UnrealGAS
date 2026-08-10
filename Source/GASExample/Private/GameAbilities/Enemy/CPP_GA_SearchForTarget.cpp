// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilities/Enemy/CPP_GA_SearchForTarget.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "CPP_EnemyCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GameplayTags/CPP_AbilityTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tasks/CPP_AbilityAsync_WaitGameplayEvent.h"
#include "Utils/CPP_BlueprintUtils.h"
#include "Tasks/AITask_MoveTo.h"

UCPP_GA_SearchForTarget::UCPP_GA_SearchForTarget()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UCPP_GA_SearchForTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	OwningEnemy = Cast<ACPP_EnemyCharacter>(GetAvatarActorFromActorInfo());
	check(OwningEnemy.IsValid());
	EnemyAIController = Cast<AAIController>(OwningEnemy->GetController());
	check(EnemyAIController.IsValid());
	StartSearch();
	WaitGameplayEventTask = UCPP_AbilityAsync_WaitGameplayEvent::WaitGameplayEventToActorProxy(GetAvatarActorFromActorInfo(), CPPAbilityTags::Event::Enemy::FinishAttack);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::OnGameplayEventReceived);
	WaitGameplayEventTask->StartActivation();
}

void UCPP_GA_SearchForTarget::StartSearch()
{
	if (!IsValid(GEngine)) return;
	if (bDrawDebug)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Searching For Target: %s"), *GetName()));
	}
	if (!OwningEnemy.IsValid()) return;
	WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, UKismetMathLibrary::RandomFloatInRange(OwningEnemy->MinAttackDelay, OwningEnemy->MaxAttackDelay));
	WaitDelayTask->OnFinish.AddDynamic(this, &ThisClass::UCPP_GA_SearchForTarget::OnAttackDelayFinished);
	WaitDelayTask->Activate();
}

void UCPP_GA_SearchForTarget::OnGameplayEventReceived(FGameplayEventData EventData)
{
	if (!OwningEnemy.IsValid() || OwningEnemy->bIsBeingLaunched) return;

	StartSearch();
}

void UCPP_GA_SearchForTarget::OnAttackDelayFinished()
{
	Search();
	if (!TargetCharacter->IsAlive())
	{
		StartSearch();
		return;
	}else
	{
		MoveAndAttack();
	}

}

void UCPP_GA_SearchForTarget::Search()
{
	auto const TargetActor = UCPP_BlueprintUtils::FindClosestActorWithTagTarget(this, OwningEnemy->GetActorLocation(), TagToFind);
	if (!TargetActor.Actor.IsValid())
	{
		StartSearch();
		return;
	}
	ACPP_BaseCharacter* BaseCharacterToFind = Cast<ACPP_BaseCharacter>(TargetActor.Actor.Get());
	if (!BaseCharacterToFind)
	{
		StartSearch();
		return;
	}
	TargetCharacter =  BaseCharacterToFind;
}

void UCPP_GA_SearchForTarget::MoveAndAttack()
{
	if (!OwningEnemy.IsValid()) return;
	if (OwningEnemy->IsAlive() == false)
	{
		StartSearch();
		return;
	}
	MoveToTask = UAITask_MoveTo::AIMoveTo(EnemyAIController.Get(), FVector::Zero(), TargetCharacter.Get(), OwningEnemy->AcceptanceRadius);
	MoveToTask->OnMoveTaskFinished.AddUObject(this, &ThisClass::AttackTarget);
	MoveToTask->ConditionalPerformMove();
}

void UCPP_GA_SearchForTarget::AttackTarget(TEnumAsByte<EPathFollowingResult::Type> Result, AAIController* AIController)
{
	if (Result != EPathFollowingResult::Success)
	{
		StartSearch();
		return;
	}
	OwningEnemy->RotateToTarget(TargetCharacter.Get());
	AttackDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, OwningEnemy->GetTimelineLength());
	AttackDelayTask->OnFinish.AddDynamic(this, &ThisClass::DoAttack);
	AttackDelayTask->Activate();
}

void UCPP_GA_SearchForTarget::DoAttack()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	const FGameplayTag AttackTag = CPPAbilityTags::Ability::Enemy::Attack;
	ASC->TryActivateAbilitiesByTag(AttackTag.GetSingleTagContainer());
}
