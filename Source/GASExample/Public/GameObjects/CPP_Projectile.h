// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Projectile.generated.h"
class UGameplayEffect;
class UProjectileMovementComponent;
UCLASS()
class GASEXAMPLE_API ACPP_Projectile : public AActor
{
	GENERATED_BODY()

public:
	ACPP_Projectile();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Crash|Damage")
	float Damage{25.f};
	UFUNCTION(BlueprintImplementableEvent, Category="Crash|Projectile")
	void SpawnImapctEffect();
protected:

	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category="Crash|Projctile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	UPROPERTY(EditDefaultsOnly, Category="Crash|Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;
};
