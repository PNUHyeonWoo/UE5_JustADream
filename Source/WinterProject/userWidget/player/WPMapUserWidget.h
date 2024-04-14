// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPMapUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPMapUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UWPMapUserWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnClickEXITButton();

	void LiftFog(uint8 YIndex, uint8 XIndex);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void GenerateRoom(TArray<class UWPArena*> Arenas);
	void GeneratePath(TArray<FVector2D> Paths);

	void LiftPathFog(FVector2D PathIndex);

	void SetBossIcon(uint8 Index);



protected:
	UPROPERTY(EditAnywhere)
	TArray<UTexture2D*> IconArray;

	UPROPERTY()
	TObjectPtr<AActor> PlayerActor;

	UPROPERTY()
	TObjectPtr<class UCanvasPanelSlot> PlayerIconSlot;

	TArray<TArray<TArray<uint8>>> RoomArray;
	
};
