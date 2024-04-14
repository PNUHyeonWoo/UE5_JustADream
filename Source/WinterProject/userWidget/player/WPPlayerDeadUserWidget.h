// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPPlayerDeadUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPPlayerDeadUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ClickEXITButton();

public:
	void SetEXP(float EXP);
	
};
