// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPTutorialExplanationUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPTutorialExplanationUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void Next();
	UFUNCTION()
	void Prev();
	void SetNewStirngTableId(FString Id);

public:
	UPROPERTY()
	TObjectPtr<class UTextBlock> TextBlockWidget;

	UPROPERTY(EditAnywhere)
	FString LastStringTableId;

	UPROPERTY(EditAnywhere)
	FString NowStringTableId;

	UPROPERTY()
	uint8 nowIndex = 0;

	UPROPERTY()
	uint8 bIsLast = 0;


	
};
