// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPStoryPageUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPStoryPageUserWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UFUNCTION()
	void OnClickSkipButton();
	UFUNCTION(BlueprintCallable)
	void OnClickImage();

protected:
	UPROPERTY(EditAnyWhere)
	TObjectPtr<class UImage> ImageWidget;

	UPROPERTY(EditAnyWhere)
	int32 NowPage;
	UPROPERTY(EditAnyWhere)
	int32 TargetPage;
	UPROPERTY(EditAnyWhere)
	TArray<UTexture2D*> Images;

	UPROPERTY(EditAnyWhere)
	TArray<class USoundBase*> Sounds;
};
