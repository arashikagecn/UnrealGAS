// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CPP_AN_MeleeAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CPP_BaseCharacter.h"
#include "CPP_PlayerCharacter.h"
#include "KismetTraceUtils.h"
#include "MeshPaintVisualize.h"
#include "GameplayTags/CPP_AbilityTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UCPP_AN_MeleeAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (!IsValid(MeshComp)) return;
	if (!IsValid(MeshComp->GetOwner())) return;
	
	TArray<FHitResult> HitResults = PerformSphereTrace(MeshComp);
	SendEventsToActors(MeshComp, HitResults);
}

TArray<FHitResult> UCPP_AN_MeleeAttack::PerformSphereTrace(USkeletalMeshComponent* MeshComp)
{
	TArray<FHitResult> Results;
	const auto SocketTransform = MeshComp->GetSocketTransform(SocketName);
	const auto Start = SocketTransform.GetLocation();
	const FVector ExtendLocation = UKismetMathLibrary::GetForwardVector(SocketTransform.GetRotation().Rotator()) * SocketExtensionOffset;
	const FVector End = Start - ExtendLocation;
	UWorld* World = GEngine->GetWorldFromContextObject(MeshComp, EGetWorldErrorMode::LogAndReturnNull);
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(MeshComp->GetOwner());
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(ActorsToIgnore);
	
	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);
	bool const bHit = IsValid(World) ? World->SweepMultiByChannel(Results, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(SphereTraceRadius), QueryParams, ResponseParams) : false;
	
	if (bDrawDebug)
	{
		DrawDebugSphereTraceMulti(World, Start, End, SphereTraceRadius, EDrawDebugTrace::ForDuration, bHit, Results, FColor::Green, FColor::Red, 5.f);
	}
	return Results;
}

void UCPP_AN_MeleeAttack::SendEventsToActors(USkeletalMeshComponent* MeshComp, const TArray<FHitResult>& HitResults) const
{

	for (const auto& HitResult : HitResults)
	{
		ACPP_PlayerCharacter* PlayerCharacter = Cast<ACPP_PlayerCharacter>(HitResult.GetActor());
		if (!PlayerCharacter) continue;
		if (!PlayerCharacter->IsAlive()) continue;
		UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
		if (!ASC) continue;
		auto EffectContent = ASC->MakeEffectContext();
		EffectContent.AddHitResult(HitResult);
		FGameplayEventData Payload;
		Payload.Target = PlayerCharacter;
		Payload.Instigator = MeshComp->GetOwner();
		Payload.ContextHandle = EffectContent;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), CPPAbilityTags::Event::Enemy::MeleeTraceHit,Payload);
		
	}
}