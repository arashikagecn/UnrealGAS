// Fill out your copyright notice in the Description page of Project Settings.

#include "Notify/CPP_ANS_ComboWindow.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameplayTags/CPP_AbilityTags.h"

void UCPP_ANS_ComboWindow::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	SendComboEvent(MeshComp, CPPAbilityTags::Event::Combo::WindowOpen);
}

void UCPP_ANS_ComboWindow::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	SendComboEvent(MeshComp, CPPAbilityTags::Event::Combo::WindowClose);
}

FString UCPP_ANS_ComboWindow::GetNotifyName_Implementation() const
{
	return TEXT("GAS Combo Window");
}

void UCPP_ANS_ComboWindow::SendComboEvent(
	USkeletalMeshComponent* MeshComp,
	const FGameplayTag& EventTag) const
{
	if (!IsValid(MeshComp))
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	if (!IsValid(Owner))
	{
		return;
	}

	FGameplayEventData Payload;
	Payload.EventTag = EventTag;
	Payload.Instigator = Owner;
	Payload.Target = Owner;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, Payload);
}
