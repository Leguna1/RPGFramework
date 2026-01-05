#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RPGAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class RPGFRAMEWORK_API URPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	URPGAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/* Health */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category="Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category="Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxHealth)

	/* Stamina */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category="Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Stamina)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category="Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxStamina)
	
	/* Adrenaline */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Adrenaline, Category="Attributes")
	FGameplayAttributeData Adrenaline;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Adrenaline)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxAdrenaline, Category="Attributes")
	FGameplayAttributeData MaxAdrenaline;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxAdrenaline)
	
	/* Experience */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Experience, Category="Attributes")
	FGameplayAttributeData Experience;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Experience)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxExperience, Category="Attributes")
	FGameplayAttributeData MaxExperience;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxExperience)

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const;
	
	UFUNCTION()
	void OnRep_Adrenaline(const FGameplayAttributeData& OldAdrenaline) const;

	UFUNCTION()
	void OnRep_MaxAdrenaline(const FGameplayAttributeData& OldMaxAdrenaline) const;
	
	UFUNCTION()
	void OnRep_Experience(const FGameplayAttributeData& OldExperience) const;

	UFUNCTION()
	void OnRep_MaxExperience(const FGameplayAttributeData& OldMaxExperience) const;
	
	
};
