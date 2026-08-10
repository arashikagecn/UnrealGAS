// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/CPP_Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CPP_PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayTags/CPP_AbilityTags.h"
#include "Utils/CPP_BlueprintUtils.h"

// Sets default values
ACPP_Projectile::ACPP_Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	
}

void ACPP_Projectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	ACPP_PlayerCharacter* PlayerCharacter = Cast<ACPP_PlayerCharacter>(OtherActor);
	if (!IsValid(PlayerCharacter)) return;
	if (!PlayerCharacter->IsAlive()) return;
	UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!IsValid(ASC) || !HasAuthority()) return;
	FGameplayEventData Payload;
	Payload.Target = PlayerCharacter;
	Payload.Instigator = GetOwner();
	UCPP_BlueprintUtils::SendDamageEventToPlayer(PlayerCharacter, DamageEffect, Payload, CPPAbilityTags::SetByCaller::Projectile, Damage);
	// FGameplayEffectContextHandle EffectHandle = ASC->MakeEffectContext();
	// FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageEffect, 1.f, EffectHandle);
	// UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, CPPAbilityTags::SetByCaller::Projectile, -Damage);
	// ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	SpawnImapctEffect();
	Destroy();
}

// Called when the game starts or when spawned
void ACPP_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}