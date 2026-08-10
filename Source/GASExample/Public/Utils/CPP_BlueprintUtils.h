// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CPP_BlueprintUtils.generated.h"

struct FGameplayTag;
struct FGameplayEventData;
struct FOverlapResult;
class UGameplayEffect;
/**
 * 
 */
UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Left,
	Right,
	Forward,
	Back,
	None
};

USTRUCT(BlueprintType)
struct FClosestActorWithTagTarget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AActor> Actor;
	UPROPERTY(BlueprintReadWrite)
	float Distance{0.f};
};

UCLASS()
class GASEXAMPLE_API UCPP_BlueprintUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Crash|Utils")
	static EHitDirection GetHitDirection(const FVector& TargetFoward, const FVector& ToInstigator);
	UFUNCTION(BlueprintPure, Category = "Crash|Utils")
	static FName GetHitDirectionName(const EHitDirection& HitDirection);
	UFUNCTION(BlueprintCallable, Category = "Crash|Utils")
	static FClosestActorWithTagTarget FindClosestActorWithTagTarget(const UObject* WorldContentObject, const FVector& Origin, const FName& Tag);
	UFUNCTION(BlueprintCallable, Category="Crash|PlayerDamage")
	static void SendDamageEventToPlayer(AActor* Target, const TSubclassOf<UGameplayEffect>& DamageEffect, UPARAM(ref) FGameplayEventData& Payload, const FGameplayTag& DataTag, float Damage, UObject* OptionalParticleSystem = nullptr);
	UFUNCTION(BlueprintCallable, Category="Crash|Overlap")
	static TArray<AActor*> HitBoxOverlapTest(AActor* AvatarActor, float HitBoxRadius, float HitBoxForwardOffset, float HitBoxEvalationOffset, bool bDrawDebug);
	UFUNCTION(BlueprintCallable, Category="Crash|Utils", meta=(AutoCreateRefTerm="TargetActors"))
	static void ApplyKonckBack(AActor* SourceActor, const TArray<AActor*>& TargetActors, float NearestDistance,
		float FarthestDistance, float ForceMagnitude, float LaunchAngle);
	static void DrawHitBoxLocation(AActor* AvatarActor, const TArray<FOverlapResult>& OverlapResults, const FVector& HitBoxLocation, float HitBoxRadius);
};
