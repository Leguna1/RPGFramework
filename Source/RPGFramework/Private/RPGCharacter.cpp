// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGCharacter.h"
#include "GAS/RPGAbilitySystemComponent.h"


// Sets default values
ARPGCharacter::ARPGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Initializer Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(FName("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	
	//Initialize Attributes
	AttributeSet = CreateDefaultSubobject<URPGAttributeSet>(FName("AttributeSet"));
	
	//Set default character level
	CharacterLevel = 1;
	
}

int32 ARPGCharacter::GetCharacterLevel() const
{
	return CharacterLevel;
}

float ARPGCharacter::GetHealth() const
{
	if (!AttributeSet)
	{
		return 0.f;
	}
	return AttributeSet->GetHealth();
}

float ARPGCharacter::GetMaxHealth() const
{
	if (!AttributeSet)
	{
		return 0.f;
	}
	return AttributeSet->GetMaxHealth();
}

float ARPGCharacter::GetStamina() const
{
	if (!AttributeSet)
	{
		return 0.f;
	}
	return AttributeSet->GetStamina();
}

float ARPGCharacter::GetMaxStamina() const
{
	if (!AttributeSet)
	{
		return 0.f;
	}
	return AttributeSet->GetMaxStamina();
}

float ARPGCharacter::GetAdrenaline() const
{
	if (!AttributeSet)
	{
		return 0.f;
	}
	return AttributeSet->GetAdrenaline();
}

float ARPGCharacter::GetMaxAdrenaline() const
{
	if (!AttributeSet)
	{
		return 0.f;
	}
	return AttributeSet->GetMaxAdrenaline();
}

float ARPGCharacter::GetExperience() const
{
	if (!AttributeSet)
	{
		return 0.f;
	}
	return AttributeSet->GetExperience();
}

float ARPGCharacter::GetMaxExperience() const
{
	if (!AttributeSet)
	{
		return 0.f;
	}
	return AttributeSet->GetMaxExperience();
}

bool ARPGCharacter::ActivateAbilitiesWithTag(FGameplayTagContainer AbilityTags, bool AllowRemoveActivation)
{
	if (!AbilitySystemComponent)
	{
		return false;
	}
	return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, AllowRemoveActivation);
}

// Called when the game starts or when spawned
void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARPGCharacter::SetTestAbilities()
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	if (GetLocalRole() == ROLE_Authority)
	{
		for (TSubclassOf<UGameplayAbility>& TestAbility : TestAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(TestAbility, GetCharacterLevel(), INDEX_NONE, this));
		}
	}
}

// Called every frame
void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARPGCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (!AbilitySystemComponent)
	{
		return;
	}
	if (EnableTestAbilities)
	{
		SetTestAbilities();
	}
	
	ApplyDefaultAttributeEffects();
	
}

UAbilitySystemComponent* ARPGCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARPGCharacter::HandleHealthChange(float DeltaValue, AActor* Causer)
{
	OnHealthChange(DeltaValue, Causer);
	if (GetHealth() <= 0.f)
	{
		OnDead();
	}
}

void ARPGCharacter::HandleStaminaChange(float DeltaValue, AActor* Causer)
{
	OnStaminaChange(DeltaValue, Causer);
	
}

void ARPGCharacter::HandleAdrenalineChange(float DeltaValue, AActor* Causer)
{
	OnAdrenalineChange(DeltaValue, Causer);
}

void ARPGCharacter::HandleExperienceChange(float DeltaValue)
{
	OnXPChange(DeltaValue);
	
}

void ARPGCharacter::HandleCharacterLevelUp()
{
	CharacterLevel += 1;
	RemoveDefaultAttributeEffects();
	ApplyDefaultAttributeEffects();
	OnCharacterLeveledUp();
}

void ARPGCharacter::ApplyDefaultAttributeEffects()
{
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffects, CharacterLevel, EffectContext);
	
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
	}
}

void ARPGCharacter::RemoveDefaultAttributeEffects()
{
	FGameplayEffectQuery Query;
	Query.EffectSource = this;
	AbilitySystemComponent->RemoveActiveEffects(Query);
}

