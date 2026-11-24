// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "CPP_ANS_ComboWindow.generated.h"

/**
 * Opens and closes the GAS combo window through gameplay events.
 * Place this notify state over the portion of the montage where input may chain.
 */
UCLASS(meta = (DisplayName = "GAS Combo Window"))
class GASEXAMPLE_API UCPP_ANS_ComboWindow : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;

private:
	void SendComboEvent(USkeletalMeshComponent* MeshComp, const FGameplayTag& EventTag) const;
};
