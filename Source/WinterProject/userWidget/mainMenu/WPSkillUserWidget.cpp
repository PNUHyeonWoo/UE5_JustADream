// Fill out your copyright notice in the Description page of Project Settings.


#include "userWidget/mainMenu/WPSkillUserWidget.h"
#include <WPGameInstance.h>
#include <Components/TextBlock.h>
#include <Engine/AssetManager.h>
#include "WPSkillBoxUserWidget.h"
#include <Components/WrapBox.h>
#include <Components/Button.h>

UWPSkillUserWidget::UWPSkillUserWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> SkillBoxWidgetClassRef(TEXT("/Game/widget/mainMenu/SkillBoxWIdget.SkillBoxWIdget_C"));
	if (SkillBoxWidgetClassRef.Class)
		SkillBoxWidgetClass = SkillBoxWidgetClassRef.Class;
	else
		UE_LOG(LogAssetData, Fatal, TEXT("Failed to load SkillBox widget subclass."));
}

void UWPSkillUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Cast<UButton>(GetWidgetFromName("EXITButton"))->OnClicked.AddDynamic(this, &UWPSkillUserWidget::SaveSkill);

	ReRenderExperience();

	UAssetManager& Manager = UAssetManager::Get();

	TArray<FAssetData> SkillAssets;
	Manager.GetPrimaryAssetDataList(TEXT("WPSkillDataAsset"), SkillAssets);

	UWrapBox* WrapBoxWidget = Cast<UWrapBox>(GetWidgetFromName("WrapBox"));

	for (FAssetData AssetData : SkillAssets)
	{
		UWPSkillBoxUserWidget* SkillBoxWidget = CreateWidget<UWPSkillBoxUserWidget>(this, SkillBoxWidgetClass, AssetData.AssetName);
		WrapBoxWidget->AddChild(SkillBoxWidget);
		SkillBoxWidget->OnLevelUp.AddUObject(this, &UWPSkillUserWidget::ReRenderExperience);
	}

	SetVisibility(ESlateVisibility::Hidden);
}

void UWPSkillUserWidget::ReRenderExperience()
{
	Cast<UTextBlock>(GetWidgetFromName("EXPValue"))->SetText(FText::FromString(FString::FromInt(Cast<UWPGameInstance>(GetGameInstance())->GetExperience())));
}

void UWPSkillUserWidget::SaveSkill()
{
	Cast<UWPGameInstance>(GetGameInstance())->SaveSkill();
	SetVisibility(ESlateVisibility::Hidden);
}
