// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGWeapon.h"

// Sets default values
ARPGWeapon::ARPGWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARPGWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARPGWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

