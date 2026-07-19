// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilities/Enemy/CPP_HitReact.h"

void UCPP_HitReact::CacheHitDirectionVectors(AActor* Instigator)
{
	AvatarForward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector InstigatorLocation = Instigator->GetActorLocation();
	const FVector AvatarLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	ToInstigator = (InstigatorLocation - AvatarLocation).GetSafeNormal();
}
