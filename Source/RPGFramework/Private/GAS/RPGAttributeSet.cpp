// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/RPGAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "RPGCharacter.h"

URPGAttributeSet::URPGAttributeSet()
{
}

void URPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(URPGAttributeSet, Health);
	DOREPLIFETIME(URPGAttributeSet, MaxHealth);
	
	DOREPLIFETIME(URPGAttributeSet, Stamina);
	DOREPLIFETIME(URPGAttributeSet, MaxStamina);
	
	DOREPLIFETIME(URPGAttributeSet, Adrenaline);
	DOREPLIFETIME(URPGAttributeSet, MaxAdrenaline);
	
	DOREPLIFETIME(URPGAttributeSet, Experience);
	DOREPLIFETIME(URPGAttributeSet, MaxExperience);
}

void URPGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	
	if (Attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute());
	}
	
	if (Attribute == GetMaxAdrenalineAttribute())
	{
		AdjustAttributeForMaxChange(Adrenaline, MaxAdrenaline, NewValue, GetAdrenalineAttribute());
	}

	if (Attribute == GetMaxExperienceAttribute())
	{
		SetExperience(0.f);
	}
	
}

void URPGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	float DeltaValue = 0;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Additive)
	{
		DeltaValue = Data.EvaluatedData.Magnitude;
	}
	
	AActor* TargetActor = nullptr;
	ARPGCharacter* RPGCharacter = nullptr;
	
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		RPGCharacter = Cast<ARPGCharacter>(TargetActor);
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		if (RPGCharacter)
		{
			RPGCharacter->HandleHealthChange(DeltaValue, Data.EffectSpec.GetContext().GetInstigator());
		}
	}

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		bool StaminaOverflow = GetStamina() > GetMaxStamina() ? FMath::IsNearlyEqual(GetStamina() - DeltaValue, GetMaxStamina()) : false;
		
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
		if (RPGCharacter && !StaminaOverflow)
		{
			RPGCharacter->HandleStaminaChange(DeltaValue, Data.EffectSpec.GetContext().GetInstigator());
		}
	}

	if (Data.EvaluatedData.Attribute == GetAdrenalineAttribute())
	{
		SetAdrenaline(FMath::Clamp(GetAdrenaline(), 0.f, GetMaxAdrenaline()));
		if (RPGCharacter)
		{
			RPGCharacter->HandleAdrenalineChange(DeltaValue, Data.EffectSpec.GetContext().GetInstigator());
		}
	}

	if (Data.EvaluatedData.Attribute == GetExperienceAttribute())
	{
		float Difference = GetExperience() - GetMaxExperience();
		if (RPGCharacter)
		{
			RPGCharacter->HandleExperienceChange(DeltaValue);
			if (Difference >= 0)
			{
				RPGCharacter->HandleCharacterLevelUp();
			}
		}
	}
}

void URPGAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty) const
{
	UAbilitySystemComponent* AbilitySystemComponent = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();

	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilitySystemComponent)
	{
		
		AbilitySystemComponent->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Override, NewMaxValue);
	}
	
}

void URPGAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Health, OldHealth);
}

void URPGAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxHealth, OldMaxHealth);
}

void URPGAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Stamina, OldStamina);
}

void URPGAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxStamina, OldMaxStamina);
}

void URPGAttributeSet::OnRep_Adrenaline(const FGameplayAttributeData& OldAdrenaline) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Adrenaline, OldAdrenaline);
}

void URPGAttributeSet::OnRep_MaxAdrenaline(const FGameplayAttributeData& OldMaxAdrenaline) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxAdrenaline, OldMaxAdrenaline);
}

void URPGAttributeSet::OnRep_Experience(const FGameplayAttributeData& OldExperience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Experience, OldExperience);
}

void URPGAttributeSet::OnRep_MaxExperience(const FGameplayAttributeData& OldMaxExperience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxExperience, OldMaxExperience);
}
