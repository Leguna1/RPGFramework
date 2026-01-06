// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCController.h"

#include "RPGCharacter.h"
#include "Perception/AIPerceptionSystem.h"

void ANPCController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	Super::SetGenericTeamId(NewTeamID);
	
	UAIPerceptionSystem::GetCurrent(GetWorld())->UpdateListener(*GetAIPerceptionComponent());
}

ETeamAttitude::Type ANPCController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<APawn>(&Other);
	if (OtherPawn == nullptr)
	{
		return ETeamAttitude::Neutral;
	}
	
	auto CharacterTeamAgent = Cast<IGenericTeamAgentInterface>(&Other);
	class IGenericTeamAgentInterface* ControllerTeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
	if (CharacterTeamAgent == nullptr && ControllerTeamAgent == nullptr)
	{
		return ETeamAttitude::Neutral;
	}
	
	FGenericTeamId OtherTeamId = FGenericTeamId();
	if (ControllerTeamAgent != nullptr)
	{
		OtherTeamId = ControllerTeamAgent->GetGenericTeamId();
	}
	else if (CharacterTeamAgent != nullptr)
	{
		OtherTeamId = CharacterTeamAgent->GetGenericTeamId();
	}

	const FGenericTeamId MyTeamId = GetGenericTeamId();
	if (OtherTeamId == EFaction::Neutral)
	{
		return ETeamAttitude::Neutral;
	}
	if (OtherTeamId == MyTeamId)
	{
		return ETeamAttitude::Friendly;
	}
	
	return ETeamAttitude::Hostile;	
}
