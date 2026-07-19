// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/CPP_BlueprintUtils.h"

#include "CPP_BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

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
