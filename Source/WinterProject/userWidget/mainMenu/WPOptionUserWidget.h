// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPOptionUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPOptionUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UWPOptionUserWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	void InitGraphicSettings();
	void SaveGraphicSettngs();

	void InitSoundSettings();
	void SaveSoundSettings();

protected:
	UFUNCTION()
	void OnClickEXITButton();
	UFUNCTION()
	void OnClickOptionButton0();
	UFUNCTION()
	void OnClickOptionButton1();
	UFUNCTION()
	void OnClickOptionButton2();
	void OnClickOptionButtonN(uint8 N);

	UFUNCTION()
	void OnClickAutoSettingButton();

	UFUNCTION()
	void OnChangeMusicSlider(float Value);
	UFUNCTION()
	void OnChangeSFXSlider(float Value);


	UFUNCTION(BlueprintNativeEvent)
	void SetMusicVolume(float Value);
	virtual void SetMusicVolume_Implementation(float Value);

	UFUNCTION(BlueprintNativeEvent)
	void SetSFXVolume(float Value);
	virtual void SetSFXVolume_Implementation(float Value);

protected:
	UPROPERTY()
	TArray<TObjectPtr<UWidget>> OptionWidgets;
	UPROPERTY()
	TObjectPtr<class UWPOptionSaveGame> OptionSaveGame;
	
};
