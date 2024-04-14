// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPChallengeUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPChallengeUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UWPChallengeUserWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void SaveChallengeWeights();

private:
	UPROPERTY()
	TSubclassOf<UUserWidget> CheckBoxWidgetClass;

	UPROPERTY()
	TSubclassOf<UUserWidget> SliderWidgetClass;
	
};
