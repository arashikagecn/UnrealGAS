// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilities/Player/CPP_GA_Primary.h"
#include "Engine/OverlapResult.h"
#include "GameplayTags/CPP_AbilityTags.h"
#include "AbilitySystemBlueprintLibrary.h"

TArray<AActor*> UCPP_GA_Primary::HitBoxOverlapTest()
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActors(ActorsToIgnore);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECollisionResponse::ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector() * HitBoxForwardOffset;
	const FVector HitBoxLocation = GetAvatarActorFromActorInfo()->GetActorLocation() + Forward + FVector(0.f, 0.f, HitBoxEvalationOffset);

	TArray<FOverlapResult> OverlapResults;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(HitBoxRadius);
	GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		HitBoxLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		CollisionShape,
		CollisionParams,
		ResponseParams
	);
	TArray<AActor*> OverlappedActors;
	for (const auto& Result : OverlapResults) {
		if (!IsValid(Result.GetActor())) continue;
		OverlappedActors.Add(Result.GetActor());
	}

	if (bDrawDebug) {
		DrawHitBoxLocation(OverlapResults, HitBoxLocation);
	}
	return OverlappedActors;
}

void UCPP_GA_Primary::SendHitReactEventToActors(const TArray<AActor*>& HitActors)
{
	for (const auto& HitActor : HitActors) {
		FGameplayEventData Payload;
		Payload.Instigator = GetAvatarActorFromActorInfo();
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, CPPAbilityTags::Event::Enemy::HitReact, Payload);
	}
}

void UCPP_GA_Primary::DrawHitBoxLocation(const TArray<FOverlapResult>& OverlapResults, const FVector& HitBoxLocation) const
{
	DrawDebugSphere(GetWorld(), HitBoxLocation, HitBoxRadius, 16, FColor::Red, false, 2.f);
	for (const auto& Result : OverlapResults) {
		if (IsValid(Result.GetActor())) {
			FVector ActorLocation = Result.GetActor()->GetActorLocation();
			ActorLocation.Z += 100.f;
			DrawDebugSphere(GetWorld(), ActorLocation, 30.f, 10, FColor::Green, false, 2.f);
		}
	}
}
