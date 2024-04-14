// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPGunTutorialUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPGunTutorialUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY();
	TObjectPtr<UWidget> NormalRealodWidget;
	UPROPERTY();
	TObjectPtr<class UCanvasPanelSlot> NormalRealodTitleSlot;
	UPROPERTY();
	TObjectPtr<class UCanvasPanelSlot> NormalRealodSlot;
	UPROPERTY();
	TObjectPtr<UWidget> FastRealodWidget;
	UPROPERTY();
	TObjectPtr<class UCanvasPanelSlot> FastRealodTitleSlot;
	UPROPERTY();
	TObjectPtr<class UCanvasPanelSlot> FastRealodSlot;
	UPROPERTY();
	TObjectPtr<UWidget> ChamberRealodWidget;
	UPROPERTY();
	TObjectPtr<class UCanvasPanelSlot> ChamberRealodTitleSlot;
	UPROPERTY();
	TObjectPtr<class UCanvasPanelSlot> ChamberRealodSlot;
	UPROPERTY();
	TObjectPtr<UWidget> PullBoltWidget;
	UPROPERTY();
	TObjectPtr<class UCanvasPanelSlot> PullBoltTitleSlot;
	UPROPERTY();
	TObjectPtr<class UCanvasPanelSlot> PullBoltSlot;
	UPROPERTY();
	TObjectPtr<UWidget> RackBoltWidget;
	UPROPERTY();
	TObjectPtr<class UCanvasPanelSlot> RackBoltTitleSlot;
	UPROPERTY();
	TObjectPtr<class UCanvasPanelSlot> RackBoltSlot;

};
