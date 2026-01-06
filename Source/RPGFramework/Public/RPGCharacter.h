// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameplayTagAssetInterface.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GAS/RPGAttributeSet.h"
#include "RPGCharacter.generated.h"

UENUM(BlueprintType)
enum EFaction : int
{
	Light = 0,
	Dark = 1, 
	Neutral = 255
};

UCLASS()
class RPGFRAMEWORK_API ARPGCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGCharacter();
	
	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetStamina() const;
	
	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetMaxStamina() const;
	
	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetAdrenaline() const;
	
	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetMaxAdrenaline() const;
	
	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetExperience() const;
	
	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetMaxExperience() const;
	
	/* Try to activate all gameplay abilities that match tag. */
	UFUNCTION(BlueprintCallable, Category = "RPG Abilities")
	bool ActivateAbilitiesWithTag(FGameplayTagContainer AbilityTags, bool AllowRemoveActivation = true);
	
	/* Called when health is changed 
	 * negative delta value for decrease, positive for increase
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChange(float DeltaValue, AActor* Causer);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnStaminaChange(float DeltaValue, AActor* Causer);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnAdrenalineChange(float DeltaValue, AActor* Causer);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnXPChange(float DeltaValue);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterLeveledUp();
	
	/* Called when character runs out of health*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnDead();
	
	UFUNCTION(BlueprintCallable, Category = "RPG Abilities | Melee")
	bool ActivateMeleeAbility(bool AllowRemoteActivation = true);
	
	UFUNCTION(BlueprintCallable, Category = "RPG Abilities")
	virtual void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UGameplayAbility*>& ActiveAbilities, bool MatchExactTag);
	
	UFUNCTION(BlueprintCallable, Category = "RPG Abilities")
	virtual void ApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect);
	
	UFUNCTION(BlueprintCallable, Category = "RPG Abilities")
	virtual bool CanApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect);
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	URPGAttributeSet* AttributeSet;
	
	/* The level of the character. It should not be changed directly once character is spawned! */
	UPROPERTY(EditAnywhere, Category = "RPG Attributes")
	int32 CharacterLevel;
	
	UPROPERTY(EditDefaultsOnly, Category = "RPG Attributes")
	TArray<TSubclassOf<UGameplayEffect>> DefaultAttributeEffects;
	
	
	/* Set this to true if you want to add test abilities to the character's ability system component. */
	UPROPERTY(EditAnywhere, Category = "RPG Attributes | Debug")
	bool EnableTestAbilities;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Attributes | Debug")
	TArray<TSubclassOf<UGameplayAbility>> TestAbilities;
	
	virtual void SetTestAbilities();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Abilities | Melee")
	TSubclassOf<UGameplayAbility> MeleeAbility;
	
	UPROPERTY()
	FGameplayAbilitySpecHandle MeleeAbilitySpecHandle;
	
	virtual void SetMeleeAbility();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team")
	TEnumAsByte<EFaction> Faction = EFaction::Neutral;
	
	FGenericTeamId TeamId;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void PossessedBy(AController* NewController) override;
	
	//Inherited via IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void HandleHealthChange(float DeltaValue, AActor* Causer);
	
	virtual void HandleStaminaChange(float DeltaValue, AActor* Causer);
	
	virtual void HandleAdrenalineChange(float DeltaValue, AActor* Causer);
	
	virtual void HandleExperienceChange(float DeltaValue);
	
	virtual void HandleCharacterLevelUp();
	
	virtual void ApplyDefaultAttributeEffects();
	
	virtual void RemoveDefaultAttributeEffects();
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
	
};
