// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Abilities/GameplayAbility.h"
#include "RPGWeapon.generated.h"

UCLASS()
class RPGFRAMEWORK_API ARPGWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPGWeapon();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Abilities")
	TSubclassOf<UGameplayAbility> LightAttackAbility;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Abilities")
	TSubclassOf<UGameplayAbility> HeavyAttackAbility;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Abilities")
	TSubclassOf<UGameplayAbility> SecondaryAbility;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
