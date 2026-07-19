// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CPP_BaseCharacter.generated.h"

namespace CPPTags
{
	extern GASEXAMPLE_API const FName Player;
}

struct FOnAttributeChangeData;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FASCInitialized, UAbilitySystemComponent*, ASC, UAttributeSet*, AS);

UCLASS(Abstract)
class GASEXAMPLE_API ACPP_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_BaseCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributesSet() const { return nullptr; }
	UPROPERTY(BlueprintAssignable, Category = "Crash|Abilities")
	FASCInitialized OnASCInitialized;

	bool IsAlive() const{ return bAlive; }
	void SetAlive(const bool AliveState) { bAlive = AliveState; };
	UFUNCTION(BlueprintCallable, Category="Crash|Death")
	virtual void HandleRespawn();
	UFUNCTION(BlueprintCallable, Category="Crash|ResetAttribute")
	virtual void ResetAttributes();
protected:
	void GiveStartupAbilities();
	void InitializeAttributes() const;
	virtual void OnHealthChanged(const FOnAttributeChangeData& AttributeChangeData);
	virtual void HandleDeath();
private:
	UPROPERTY(EditDefaultsOnly, Category = "Crash|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "Crash|Effects")
	TSubclassOf<UGameplayEffect> InitialAttributesEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Crash|Effects")
	TSubclassOf<UGameplayEffect> ResetAttributesEffect;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true) , Category = "Crash|State")
	bool bAlive = true;
};
