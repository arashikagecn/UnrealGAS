// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_EnemyCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "GameAbilities/CPP_AbilitySystemComponent.h"
#include "Attributes/CPP_AttributeSet.h"
#include "GameplayTags/CPP_AbilityTags.h"
#include "Net/UnrealNetwork.h"

ACPP_EnemyCharacter::ACPP_EnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	AbilitySystemComponent = CreateDefaultSubobject<UCPP_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UCPP_AttributeSet>("AttributeSet");
}

void ACPP_EnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPP_EnemyCharacter, bIsBeingLaunched);
}

UAbilitySystemComponent* ACPP_EnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACPP_EnemyCharacter::StopMovementUntilLanded()
{
	if (!HasAuthority()) return;

	bIsBeingLaunched = true;

	if (AAIController* AIController = GetController<AAIController>())
	{
		AIController->StopMovement();
	}

	LandedDelegate.AddUniqueDynamic(this, &ThisClass::EnableMovementWhenLanded);
}

void ACPP_EnemyCharacter::EnableMovementWhenLanded(const FHitResult& Hit)
{
	bIsBeingLaunched = false;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, CPPAbilityTags::Event::Enemy::FinishAttack, FGameplayEventData());
	LandedDelegate.RemoveDynamic(this, &ThisClass::EnableMovementWhenLanded);
}

void ACPP_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (!IsValid(GetAbilitySystemComponent())) return;
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	OnASCInitialized.Broadcast(AbilitySystemComponent, AttributeSet);
	if (!HasAuthority()) return;

	GiveStartupAbilities();
	InitializeAttributes();
	UCPP_AttributeSet* AttributesSet = Cast<UCPP_AttributeSet>(GetAttributesSet());
	if (!IsValid(AttributeSet)) return;
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(AttributesSet->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
}

void ACPP_EnemyCharacter::HandleDeath()
{
	Super::HandleDeath();
	AAIController* AIController = GetController<AAIController>();
	if (!IsValid(AIController)) return;
	AIController->StopMovement();
}
