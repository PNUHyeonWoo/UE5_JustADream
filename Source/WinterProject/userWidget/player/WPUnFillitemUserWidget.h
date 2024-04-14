// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPUnFillitemUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPUnFillitemUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
};
