// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPMoveTutorialUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPMoveTutorialUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
};
