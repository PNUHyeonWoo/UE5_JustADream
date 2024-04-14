// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPSkillUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPSkillUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UWPSkillUserWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void ReRenderExperience();

	UFUNCTION()
	void SaveSkill();

private:

	UPROPERTY()
	TSubclassOf<UUserWidget> SkillBoxWidgetClass;
	
};
