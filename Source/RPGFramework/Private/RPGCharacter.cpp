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

bool ARPGCharacter::ActivateMeleeAbility(bool AllowRemoteActivation)
{
	if (!AbilitySystemComponent || !MeleeAbilitySpecHandle.IsValid())
	{
		return false;
	}
	return AbilitySystemComponent->TryActivateAbility(MeleeAbilitySpecHandle);
}

void ARPGCharacter::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags,
	TArray<UGameplayAbility*>& ActiveAbilities, bool MatchExactTag)
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	TArray<FGameplayAbilitySpec*> MatchingAbilities;
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTags, MatchingAbilities, MatchExactTag);
	for (FGameplayAbilitySpec* Spec : MatchingAbilities)
	{
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();
		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(ActiveAbility);
		}
	}
}

void ARPGCharacter::ApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect)
{
	if (!AbilitySystemComponent || !GameplayEffect)
	{
		return;
	}
	
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	
	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, CharacterLevel, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
	}
	
}

bool ARPGCharacter::CanApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect)
{
	if (!AbilitySystemComponent || !GameplayEffect)
	{
		return false;
	}
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	
	return AbilitySystemComponent->CanApplyAttributeModifiers(GameplayEffect->GetDefaultObject<UGameplayEffect>(), CharacterLevel, EffectContext);
}

bool ARPGCharacter::EquipWeapon(ARPGWeapon* Weapon, TEnumAsByte<EWeaponSlot> EquipSlot)
{
	if (!Weapon)
	{
		return false;
	}
	switch (EquipSlot)
	{
	case EWeaponSlot::RightHand:
		EquipRightHand(Weapon);
		break;
	case EWeaponSlot::LeftHand:
		EquipLeftHand(Weapon);
		break;
	default:
		break;
		
	}
	
	return true;
}

bool ARPGCharacter::ActivateAbilityBySlot(TEnumAsByte<EAbilitySlot> AbilitySlot, bool AllowRemoteActivation)
{
	if (!AbilitySystemComponent || !SlotAbilityHandles.Contains(AbilitySlot))
	{
		return false;
	}
	
	FGameplayAbilitySpecHandle* SpecHandle = SlotAbilityHandles.Find(AbilitySlot);
	return AbilitySystemComponent->TryActivateAbility(FGameplayAbilitySpecHandle(*SpecHandle), AllowRemoteActivation);
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

void ARPGCharacter::SetMeleeAbility()
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	MeleeAbilitySpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(MeleeAbility, GetCharacterLevel(),INDEX_NONE, this));
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
	TeamId = FGenericTeamId(Faction);
	
	if (!AbilitySystemComponent)
	{
		return;
	}
	if (EnableTestAbilities)
	{
		SetTestAbilities();
	}
	
	ApplyDefaultAttributeEffects();
	SetMeleeAbility();
	
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
	
	for (TSubclassOf<UGameplayEffect>& DefaultEffect : DefaultAttributeEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultEffect, CharacterLevel, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}
}

void ARPGCharacter::RemoveDefaultAttributeEffects()
{
	FGameplayEffectQuery Query;
	Query.EffectSource = this;
	AbilitySystemComponent->RemoveActiveEffects(Query);
}
void ARPGCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
	}
}

void ARPGCharacter::ClearAbilitySlot(TEnumAsByte<EAbilitySlot> AbilitySlot)
{
	if (SlotAbilityHandles.IsEmpty())
	{
		return;
	}
	if (SlotAbilityHandles.Contains(AbilitySlot))
	{
		FGameplayAbilitySpecHandle* SpecHandle = SlotAbilityHandles.Find(AbilitySlot);
		AbilitySystemComponent->ClearAbility(*SpecHandle);
		SlotAbilityHandles.Remove(AbilitySlot);
	}
}

void ARPGCharacter::AddAbilityToSlot(TSubclassOf<UGameplayAbility> NewAbility, TEnumAsByte<EAbilitySlot> AbilitySlot)
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(NewAbility, GetCharacterLevel(), INDEX_NONE, this));
	if (SpecHandle.IsValid())
	{
		SlotAbilityHandles.Add(AbilitySlot, SpecHandle);
	}
}

void ARPGCharacter::EquipRightHand(ARPGWeapon* Weapon)
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	ClearAbilitySlot(EAbilitySlot::LightAttack);
	ClearAbilitySlot(EAbilitySlot::HeavyAttack);
	AddAbilityToSlot(Weapon->LightAttackAbility, EAbilitySlot::LightAttack);
	AddAbilityToSlot(Weapon->HeavyAttackAbility, EAbilitySlot::HeavyAttack);
}

void ARPGCharacter::EquipLeftHand(ARPGWeapon* Weapon)
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	ClearAbilitySlot(EAbilitySlot::SecondaryAbility);
	AddAbilityToSlot(Weapon->SecondaryAbility, EAbilitySlot::SecondaryAbility);
}
