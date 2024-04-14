// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPShopUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPShopUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	

protected:
	UFUNCTION()
	void ClickExitButton();
	UFUNCTION()
	void ClickGearUpgradeButton();
	UFUNCTION(BlueprintCallable)
	void GiveMoneyToPlayer();

	static const uint32 GearUpgradePrice[];
	
};
