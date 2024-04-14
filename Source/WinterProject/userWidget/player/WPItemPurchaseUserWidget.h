// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPItemPurchaseUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPItemPurchaseUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void PurchaseButtonClick();
	
};
