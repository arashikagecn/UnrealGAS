// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/CPP_AbilityAsync_WaitGameplayEvent.h"

void UCPP_AbilityAsync_WaitGameplayEvent::StartActivation()
{
	Activate();
}

UCPP_AbilityAsync_WaitGameplayEvent* UCPP_AbilityAsync_WaitGameplayEvent::WaitGameplayEventToActorProxy(AActor* TargetActor,
	FGameplayTag EventTag, bool OnlyTriggerOnce, bool OnlyMatchExact)
{
	UCPP_AbilityAsync_WaitGameplayEvent* MyObj = NewObject<UCPP_AbilityAsync_WaitGameplayEvent>();
	MyObj->SetAbilityActor(TargetActor);
	MyObj->Tag = EventTag;
	MyObj->OnlyTriggerOnce = OnlyTriggerOnce;
	MyObj->OnlyMatchExact = OnlyMatchExact;
	return MyObj;
}
