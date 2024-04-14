// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPGunPurchaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPGunPurchaseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void PurchaseButtonClick();

protected:
	UPROPERTY()
	TObjectPtr<class UButton> PurchaseButtonWidget;
	
};
