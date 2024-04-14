// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WPLoadLevelGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API AWPLoadLevelGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
};
