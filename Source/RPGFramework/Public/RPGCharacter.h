// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GAS/RPGAttributeSet.h"
#include "RPGCharacter.generated.h"

UCLASS()
class RPGFRAMEWORK_API ARPGCharacter : public ACharacter, public IAbilitySystemInterface
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
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	URPGAttributeSet* AttributeSet;
	
	/* The level of the character. It should not be changed directly once character is spawned! */
	UPROPERTY(EditAnywhere, Category = "RPG Abilities")
	int32 CharacterLevel;
	
	UPROPERTY(EditDefaultsOnly, Category = "RPG Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffects;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void PossessedBy(AController* NewController) override;
	
	//Inherited via IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

};
