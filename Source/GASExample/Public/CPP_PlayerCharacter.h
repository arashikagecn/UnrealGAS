// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BaseCharacter.h"
#include "CPP_PlayerCharacter.generated.h"


/**
 * 
 */
class USpringArmComponent;
class UCameraComponent;
UCLASS()
class GASEXAMPLE_API ACPP_PlayerCharacter : public ACPP_BaseCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ACPP_PlayerCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual UAttributeSet* GetAttributesSet() const override;
private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;
};
