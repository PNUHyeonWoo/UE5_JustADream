// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WPMainMenuLevelGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API AWPMainMenuLevelGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWPMainMenuLevelGameMode();


protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	
};
