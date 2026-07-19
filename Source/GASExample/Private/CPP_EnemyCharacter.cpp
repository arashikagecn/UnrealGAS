// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_EnemyCharacter.h"
#include "GameAbilities/CPP_AbilitySystemComponent.h"
#include "Attributes/CPP_AttributeSet.h"

ACPP_EnemyCharacter::ACPP_EnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	AbilitySystemComponent = CreateDefaultSubobject<UCPP_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UCPP_AttributeSet>("AttributeSet");
}

UAbilitySystemComponent* ACPP_EnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
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
