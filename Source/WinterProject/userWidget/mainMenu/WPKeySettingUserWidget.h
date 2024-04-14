// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPKeySettingUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPKeySettingUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
protected:
	UPROPERTY(EditAnyWhere)
	FString MappingName;
	UPROPERTY(EditAnyWhere)
	uint8 bIsAxisMapping : 1;
	UPROPERTY(EditAnyWhere)
	float AxisValue;
	
};
