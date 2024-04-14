// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPSkillBoxUserWidget.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnLevelUp);


UCLASS()
class WINTERPROJECT_API UWPSkillBoxUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnClickLevelUpButton();

public:
	FOnLevelUp OnLevelUp;

	UPROPERTY()
	TObjectPtr<class UWPSkillDataAsset> SkillDataAsset;
	
};
