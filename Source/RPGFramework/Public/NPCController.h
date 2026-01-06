// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCController.generated.h"

/**
 * 
 */
UCLASS()
class RPGFRAMEWORK_API ANPCController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	
protected:
	
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
};
