// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPMainMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPMainMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnClickPlayButton();

	UFUNCTION()
	void OnClickTutorialButton();

	UFUNCTION()
	void OnClickChallengeButton();

	UFUNCTION()
	void OnClickSkillButton();

	UFUNCTION()
	void OnClickOptionButton();

	UFUNCTION()
	void GetExperience1000();

	bool CanPlayGame();
	
};
