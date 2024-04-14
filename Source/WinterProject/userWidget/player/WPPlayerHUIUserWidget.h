// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPPlayerHUIUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPPlayerHUIUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UWPPlayerHUIUserWidget(const FObjectInitializer& ObjectInitializer);

public:
	void RefreshGear(uint8 GearPart);
	void OpenShop();
	void HideShop();
	void CreateStatusEffectImages(TMap<FName,float> StatusEffects);
	void SetStatusEffectImageVisiblity(FName StatusEffectName, bool Value);
	void OpenPlayerDeadWidget();
	void GenerateRoomForMap(TArray<class UWPArena*> Arenas);
	void GeneratePathForMap(TArray<FVector2D> Paths);
	void ToggleMap();
	void ToggleMainMenuPanel();
	void SetEXPText(float EXP);
	void SetStaminaBarCanRun(bool Value);
	void SetLoadingScreen(bool Value);
	void SetRemainAmmoVisible(float RemainAmmo);
	void SetRemainMagazineAmmoVisible(float RemainAmmo);
	void LiftPathFog(FVector2D PathIndex);
	void SetBossIcon(uint8 Index);
	void SetTutorialWidget(FString Id);

protected:
	UFUNCTION()
	void OnClickMainMenuButton();
	UFUNCTION()
	void OnClickMainMenuEXITButton();
	UFUNCTION()
	void OnClickOptionButton();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry,float InDeltaTime) override;
	void SetGold(uint32 Value);

	UPROPERTY()
	TObjectPtr<class UCanvasPanel> CursorPanelWidget;

	UPROPERTY()
	TObjectPtr<class UImage> CursorImageWidget;

	UPROPERTY()
	TSubclassOf<class UUserWidget> FillItemWidgetClass;

	UPROPERTY()
	TSubclassOf<class UUserWidget> UnFillItemWidgetClass;

	UPROPERTY()
	TObjectPtr<UTexture2D> EmptyItemImage;


	UPROPERTY()
	TObjectPtr<UTexture2D> NonTrueAimCursorImage;

	UPROPERTY()
	TObjectPtr<UTexture2D> TrueAimCursorImage;

	UPROPERTY()
	TObjectPtr<class UProgressBar> HpBarWidget;

	UPROPERTY()
	TObjectPtr<class UProgressBar> StaminaBarWidget;

	UPROPERTY()
	TObjectPtr<class UTextBlock> GoldTextBlock;

	UPROPERTY()
	TObjectPtr<class UWPShopUserWidget> ShopWidget;

	UPROPERTY()
	uint8 bIsFadeIn;

	UPROPERTY()
	TObjectPtr<class UImage> LoadingImageWidget;



};
