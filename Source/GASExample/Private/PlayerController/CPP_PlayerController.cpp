// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/CPP_PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTags/CPP_AbilityTags.h"

void ACPP_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(InputSubsystem)) return;
	for (auto content : InputMappingContexts) {
		InputSubsystem->AddMappingContext(content, 0);
	}
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(EnhancedInputComponent)) return;
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACPP_PlayerController::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACPP_PlayerController::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPP_PlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPP_PlayerController::Look);
	EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Started, this, &ACPP_PlayerController::CastPrimaryAction);
	EnhancedInputComponent->BindAction(SecondaryAction, ETriggerEvent::Started, this, &ACPP_PlayerController::CastSecondaryActionAction);
	EnhancedInputComponent->BindAction(TertiaryAction, ETriggerEvent::Started, this, &ACPP_PlayerController::CastTertiaryAction);
}

void ACPP_PlayerController::Jump()
{
	if (!IsValid(GetCharacter())) return;
	GetCharacter()->Jump();
}
void ACPP_PlayerController::StopJumping()
{
	if (!IsValid(GetCharacter())) return;
	GetCharacter()->StopJumping();
}

void ACPP_PlayerController::Move(const FInputActionValue& Value)
{
	if (!IsValid(GetPawn())) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	GetPawn()->AddMovementInput(ForwardDirection, MovementVector.Y);
	GetPawn()->AddMovementInput(RightDirection, MovementVector.X);
}

void ACPP_PlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void ACPP_PlayerController::CastPrimaryAction()
{
	//UE_LOG(LogTemp, Warning, TEXT("PrimaryAction active"));
	ActivateAbilities(CPPAbilityTags::Ability::Primary);
}

void ACPP_PlayerController::CastSecondaryActionAction()
{
	ActivateAbilities(CPPAbilityTags::Ability::Secondary);
}

void ACPP_PlayerController::CastTertiaryAction()
{
	ActivateAbilities(CPPAbilityTags::Ability::Tertiary);
}

void ACPP_PlayerController::ActivateAbilities(const FGameplayTag& AbilityTag)
{
	UAbilitySystemComponent* AbsComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetCharacter());
	if (!IsValid(AbsComponent)) return;
	AbsComponent->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
}
