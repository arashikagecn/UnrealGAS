// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/CPP_BlueprintUtils.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CPP_BaseCharacter.h"
#include "CPP_PlayerCharacter.h"
#include "Attributes/Cpp_AttributeSet.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "GameplayTags/CPP_AbilityTags.h"
#include "Kismet/GameplayStatics.h"
#include "Tasks/CPP_AbilityAsync_WaitGameplayEvent.h"

EHitDirection UCPP_BlueprintUtils::GetHitDirection(const FVector& TargetFoward, const FVector& ToInstigator)
{
	const float Dot = FVector::DotProduct(TargetFoward, ToInstigator);
	if (Dot < -0.5f) {
		return EHitDirection::Back;
	}
	if (Dot < 0.5f) {
		const FVector Cross = FVector::CrossProduct(TargetFoward, ToInstigator);
		if (Cross.Z < 0.f) {
			return EHitDirection::Left;
		}
		return EHitDirection::Right;
	}
	return EHitDirection::Forward;
}

FName UCPP_BlueprintUtils::GetHitDirectionName(const EHitDirection& HitDirection)
{
	switch (HitDirection)
	{
	case EHitDirection::Forward:return FName("Forward");
	case EHitDirection::Back:return FName("Back");
	case EHitDirection::Left:return FName("Left");
	case EHitDirection::Right:return FName("Right");
	default:
		return FName("None");
	}
}

FClosestActorWithTagTarget UCPP_BlueprintUtils::FindClosestActorWithTagTarget(const UObject* WorldContentObject,
	const FVector& Origin, const FName& Tag)
{
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(WorldContentObject, Tag, ActorsWithTag);
	float fClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;
	for (AActor* Actor : ActorsWithTag)
	{
		if (!IsValid(Actor)) continue;
		ACPP_BaseCharacter* Character = Cast<ACPP_BaseCharacter>(Actor);
		if (!IsValid(Character) || !Character->IsAlive()) continue;
		
		const float Distance = FVector::Dist(Origin, Character->GetActorLocation());
		if (Distance < fClosestDistance)
		{
			fClosestDistance = Distance;
			ClosestActor = Actor;
		}
	}
	return FClosestActorWithTagTarget(ClosestActor, fClosestDistance);
}

void UCPP_BlueprintUtils::SendDamageEventToPlayer(AActor* Target, const TSubclassOf<UGameplayEffect>& DamageEffect,
		FGameplayEventData& Payload, const FGameplayTag& DataTag, float Damage, UObject* OptionalParticleSystem)
{
	ACPP_BaseCharacter* PlayerCharacter = Cast<ACPP_BaseCharacter>(Target);
	if (!IsValid(PlayerCharacter)) return;
	if (!PlayerCharacter->IsAlive()) return;
	
	UCPP_AttributeSet* Attributes =Cast<UCPP_AttributeSet>(PlayerCharacter->GetAttributesSet());
	if (!IsValid(Attributes)) return;
	const bool bLethal = Attributes->GetHealth() <= Damage;
	const FGameplayTag PlayerTag = bLethal ? CPPAbilityTags::Event::Player::Death : CPPAbilityTags::Event::Player::HitReact;
	Payload.OptionalObject = OptionalParticleSystem;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Target, PlayerTag, Payload);
	
	UAbilitySystemComponent* TargetASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!IsValid(TargetASC)) return; 
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(DamageEffect, 1.f, EffectContextHandle);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, DataTag, -Damage);
	
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

TArray<AActor*> UCPP_BlueprintUtils::HitBoxOverlapTest(AActor* AvatarActor, float HitBoxRadius, float HitBoxForwardOffset, float HitBoxEvalationOffset, bool bDrawDebug)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarActor);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActors(ActorsToIgnore);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECollisionResponse::ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	const FVector Forward = AvatarActor->GetActorForwardVector() * HitBoxForwardOffset;
	const FVector HitBoxLocation = AvatarActor->GetActorLocation() + Forward + FVector(0.f, 0.f, HitBoxEvalationOffset);

	TArray<FOverlapResult> OverlapResults;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(HitBoxRadius);
	UWorld* World = AvatarActor->GetWorld();
	if (!IsValid(World)) return {};
	World->OverlapMultiByChannel(
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
		DrawHitBoxLocation(AvatarActor, OverlapResults, HitBoxLocation, HitBoxRadius);
	}
	return OverlappedActors;
}

void UCPP_BlueprintUtils::ApplyKonckBack(AActor* SourceActor, const TArray<AActor*>& TargetActors,
	float NearestDistance, float FarthestDistance, float ForceMagnitude, float LaunchAngle)
{
	if (!IsValid(SourceActor) || ForceMagnitude <= 0.f)
	{
		return;
	}

	const float NearDistance = FMath::Max(0.f, NearestDistance);
	const float FarDistance = FMath::Max(NearDistance, FarthestDistance);
	const float AngleInRadians = FMath::DegreesToRadians(FMath::Clamp(LaunchAngle, -90.f, 90.f));
	const FVector SourceLocation = SourceActor->GetActorLocation();

	for (AActor* TargetActor : TargetActors)
	{
		if (!IsValid(TargetActor) || TargetActor == SourceActor)
		{
			continue;
		}

		const FVector ToTarget = TargetActor->GetActorLocation() - SourceLocation;
		const float Distance = ToTarget.Size();
		if (Distance > FarDistance)
		{
			continue;
		}

		const float AppliedMagnitude = FMath::IsNearlyEqual(NearDistance, FarDistance)
			? ForceMagnitude
			: FMath::GetMappedRangeValueClamped(
				FVector2D(NearDistance, FarDistance),
				FVector2D(ForceMagnitude, 0.f),
				Distance);
		if (AppliedMagnitude <= UE_KINDA_SMALL_NUMBER)
		{
			continue;
		}

		FVector KnockbackForce = ToTarget.GetSafeNormal();
		if (KnockbackForce.IsNearlyZero())
		{
			KnockbackForce = SourceActor->GetActorForwardVector().GetSafeNormal();
		}
		KnockbackForce.Z = 0.f;
		const FVector Right = KnockbackForce.RotateAngleAxis(90.f, FVector::UpVector);
		KnockbackForce = KnockbackForce.RotateAngleAxis(-LaunchAngle, Right) * AppliedMagnitude;
		if (ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor))
		{
			TargetCharacter->LaunchCharacter(KnockbackForce , true, true);
		}
	}
}

void UCPP_BlueprintUtils::DrawHitBoxLocation(AActor* AvatarActor, const TArray<FOverlapResult>& OverlapResults, const FVector& HitBoxLocation, float HitBoxRadius)
{
	UWorld* World = AvatarActor->GetWorld();
	if (!IsValid(World)) return;
	DrawDebugSphere(World, HitBoxLocation, HitBoxRadius, 16, FColor::Red, false, 2.f);
	for (const auto& Result : OverlapResults) {
		if (IsValid(Result.GetActor())) {
			FVector ActorLocation = Result.GetActor()->GetActorLocation();
			ActorLocation.Z += 100.f;
			DrawDebugSphere(World, ActorLocation, 30.f, 10, FColor::Green, false, 2.f);
		}
	}
}
