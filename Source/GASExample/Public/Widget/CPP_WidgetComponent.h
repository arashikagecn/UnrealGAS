// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "AttributeSet.h"
#include "CPP_WidgetComponent.generated.h"

/**
 * 
 */
class ACPP_BaseCharacter;
class UCPP_AbilitySystemComponent;
class UCPP_AttributeSet;
class UAbilitySystemComponent;
UCLASS(ClassGroup=(Custon), meta=(BlueprintSpawnableComponent))
class GASEXAMPLE_API UCPP_WidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	TMap<FGameplayAttribute, FGameplayAttribute> AttributeMap;
private:
	TWeakObjectPtr<ACPP_BaseCharacter> OwnerCharacter;
	TWeakObjectPtr<UCPP_AbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<UCPP_AttributeSet> AttributeSet;

	void InitAbilitySystemData();
	bool IsASCInitialized() const;

	UFUNCTION()
	void OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
	UFUNCTION()
	void BindToAttributesChange();
	UFUNCTION()	
	void InitializeAttributeDelegate();
	
	void BindWidgetAttributeChanges(UWidget* WidgetObject, const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const;
};
