// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/CPP_AttributeChangeTask.h"
#include "AbilitySystemComponent.h"

UCPP_AttributeChangeTask* UCPP_AttributeChangeTask::ListenForAttributeChange(
	UAbilitySystemComponent* AbilitySystemComponent,const FGameplayAttribute AttributeInput)
{
	if (!IsValid(AbilitySystemComponent))
	{
		return nullptr;
	}
	UCPP_AttributeChangeTask* TaskHandler = NewObject<UCPP_AttributeChangeTask>();	
	TaskHandler->ASC = AbilitySystemComponent;
	TaskHandler->AttributeListenFor = AttributeInput;
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeInput).AddUObject(TaskHandler, &UCPP_AttributeChangeTask::AttributeChanged);
	return TaskHandler;
}

void UCPP_AttributeChangeTask::EndTask()
{
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(AttributeListenFor).RemoveAll(this);
	}
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UCPP_AttributeChangeTask::AttributeChanged(const FOnAttributeChangeData& Data)
{
	OnAttributeChange.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
}
 