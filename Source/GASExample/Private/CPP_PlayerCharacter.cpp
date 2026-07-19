// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h" // ���Ӵ����԰���UCapsuleComponent����������
#include "GameFramework/CharacterMovementComponent.h" // ���Ӵ����԰���UCharacterMovementComponent����������
#include "AbilitySystemComponent.h"
#include "Attributes/CPP_AttributeSet.h"
#include "Player/CPP_PlayerState.h"

ACPP_PlayerCharacter::ACPP_PlayerCharacter()
{
	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//TObjectPtr<UCharacterMovementComponent> CharacterMovement = GetCharacterMovement();
	//CharacterMovement->bOrientRotationToMovement = true; // Character moves in the direction of input...
	//CharacterMovement->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	//CharacterMovement->JumpZVelocity = 500.f;
	//CharacterMovement->AirControl = 0.35f;
	//CharacterMovement->MaxWalkSpeed = 500.f;
	//CharacterMovement->MinAnalogWalkSpeed = 20.f;
	//CharacterMovement->BrakingDecelerationWalking = 2000.f;
	//CharacterMovement->BrakingDecelerationFalling = 1500.f;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	Tags.Add(CPPTags::Player);
}

UAbilitySystemComponent* ACPP_PlayerCharacter::GetAbilitySystemComponent() const
{
	auto asc = GetPlayerState<ACPP_PlayerState>()->GetAbilitySystemComponent();
	if (!IsValid(asc)) return nullptr;
	return asc;
}

void ACPP_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (!IsValid(GetAbilitySystemComponent()) || !HasAuthority()) return;
	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState<ACPP_PlayerState>(), this);
	OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributesSet());
	GiveStartupAbilities();
	InitializeAttributes();
	UCPP_AttributeSet* AttributeSet = Cast<UCPP_AttributeSet>(GetAttributesSet());
	if (!IsValid(AttributeSet)) return;
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
}

void ACPP_PlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	if (!IsValid(GetAbilitySystemComponent())) return;
	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState<ACPP_PlayerState>(), this);
	
	UCPP_AttributeSet* AttributeSet = Cast<UCPP_AttributeSet>(GetAttributesSet());
	if (!IsValid(AttributeSet)) return;
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
}

UAttributeSet* ACPP_PlayerCharacter::GetAttributesSet() const
{
	auto* CPP_PlayerState = GetPlayerState<ACPP_PlayerState>();
	if (!IsValid(CPP_PlayerState)) return nullptr;
	return CPP_PlayerState->GetAttributesSet();
}
