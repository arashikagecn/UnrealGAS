// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CPP_AttributeChangeTask.generated.h"

struct FOnAttributeChangeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChange, FGameplayAttribute, Attribute, float, NewValue,
                                               float, OldValue);
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class GASEXAMPLE_API UCPP_AttributeChangeTask : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChange OnAttributeChange;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UCPP_AttributeChangeTask* ListenForAttributeChange(UAbilitySystemComponent* AbilitySystemComponent,const FGameplayAttribute AttributeInput);
	UFUNCTION(BlueprintCallable)
	void EndTask();
	TWeakObjectPtr<UAbilitySystemComponent> ASC;
	FGameplayAttribute AttributeListenFor;
	
	void AttributeChanged(const FOnAttributeChangeData& Data);
};
