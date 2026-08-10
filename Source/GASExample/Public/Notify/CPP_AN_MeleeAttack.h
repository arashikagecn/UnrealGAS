// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CPP_AN_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class GASEXAMPLE_API UCPP_AN_MeleeAttack : public UAnimNotifyState
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category="Crash|Debug")
	bool bDrawDebug;
	UPROPERTY(EditAnywhere, Category="Crash|Socket")
	FName SocketName{""};
	UPROPERTY(EditAnywhere, Category="Crash|Socket")
	float SocketExtensionOffset{40.f};
	UPROPERTY(EditAnywhere, Category="Crash|Socket")
	float SphereTraceRadius{60.f};
	
	void SendEventsToActors(USkeletalMeshComponent* MeshComp, const TArray<FHitResult>& HitResults) const;
public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	
	TArray<FHitResult> PerformSphereTrace(USkeletalMeshComponent* MeshComp);
};
