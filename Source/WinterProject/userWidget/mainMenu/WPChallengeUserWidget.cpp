// Fill out your copyright notice in the Description page of Project Settings.


#include "userWidget/mainMenu/WPChallengeUserWidget.h"
#include <Engine/AssetManager.h>
#include <dataAsset/WPChallengeDataAsset.h>
#include "Components/HorizontalBox.h" 
#include "Components/Checkbox.h"
#include "Components/VerticalBox.h"
#include "WPGameInstance.h"
#include <Components/Slider.h>
#include <Components/TextBlock.h>
#include <Components/Button.h>

UWPChallengeUserWidget::UWPChallengeUserWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
    static ConstructorHelpers::FClassFinder<UUserWidget> CheckBoxWidgetClassRef(TEXT("/Game/widget/mainMenu/ChallengeCheckBoxRow.ChallengeCheckBoxRow_C"));
    if (CheckBoxWidgetClassRef.Class)
        CheckBoxWidgetClass = CheckBoxWidgetClassRef.Class;
    else
        UE_LOG(LogAssetData, Fatal, TEXT("Failed to load CheckBox widget subclass."));

    static ConstructorHelpers::FClassFinder<UUserWidget> SliderWidgetClassRef(TEXT("/Game/widget/mainMenu/ChallengeSliderRow.ChallengeSliderRow_C"));
    if (SliderWidgetClassRef.Class)
        SliderWidgetClass = SliderWidgetClassRef.Class;
    else
        UE_LOG(LogAssetData, Fatal, TEXT("Failed to load Slider widget subclass."));
}

void UWPChallengeUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    //EXIT 버튼 바인딩
    
    Cast<UButton>(GetWidgetFromName("EXITButton"))->OnClicked.AddDynamic(this, &UWPChallengeUserWidget::SaveChallengeWeights);


    UAssetManager& Manager = UAssetManager::Get();

    TArray<FAssetData> ChallengeAssets;
    Manager.GetPrimaryAssetDataList(TEXT("WPChallengeDataAsset"), ChallengeAssets);

    
    UVerticalBox* VerticalBox = Cast<UVerticalBox>(GetWidgetFromName(TEXT("VerticalBox")));


    for (FAssetData AssetData : ChallengeAssets)
    {
         UWPChallengeDataAsset* Asset = Cast<UWPChallengeDataAsset>(AssetData.GetAsset());


         if (Asset->maxWeight <= 1) // 체크박스 생성
         {
             UUserWidget* Widget = CreateWidget<UUserWidget>(this, CheckBoxWidgetClass,AssetData.AssetName);
             VerticalBox->AddChild(Widget);
             Cast<UCheckBox>(Widget->GetWidgetFromName("CheckBox"))->SetIsChecked(Cast<UWPGameInstance>(GetGameInstance())->GetChallengeWeight(AssetData.AssetName) != 0);
             Cast<UTextBlock>(Widget->GetWidgetFromName("Text"))->SetText(FText::FromName(AssetData.AssetName));
         }
         else //슬라이드 생성
         {
             UUserWidget* Widget = CreateWidget<UUserWidget>(this, SliderWidgetClass, AssetData.AssetName);
             VerticalBox->AddChild(Widget);
             USlider* Slider = Cast<USlider>(Widget->GetWidgetFromName("Slider"));
             Slider->SetMaxValue(Asset->maxWeight);
             Slider->SetValue(Cast<UWPGameInstance>(GetGameInstance())->GetChallengeWeight(AssetData.AssetName));
             Cast<UTextBlock>(Widget->GetWidgetFromName("Text"))->SetText(FText::FromName(AssetData.AssetName));
         }
    }
        
    SetVisibility(ESlateVisibility::Hidden);
        
}

void UWPChallengeUserWidget::SaveChallengeWeights()
{
    UAssetManager& Manager = UAssetManager::Get();


    TArray<FAssetData> ChallengeAssets;
    Manager.GetPrimaryAssetDataList(TEXT("WPChallengeDataAsset"), ChallengeAssets);

    for (FAssetData AssetData : ChallengeAssets)
    {
        UWidget* Widget = GetWidgetFromName(AssetData.AssetName);
        uint8 value = 0;

        if (Widget->IsA(CheckBoxWidgetClass))
            value = Cast<UCheckBox>(Cast<UUserWidget>(Widget)->GetWidgetFromName("CheckBox"))->IsChecked() ? 1 : 0;
        else
            value = Cast<USlider>(Cast<UUserWidget>(Widget)->GetWidgetFromName("Slider"))->Value;

        Cast<UWPGameInstance>(GetGameInstance())->SetChallengeWeight(AssetData.AssetName, value);
    }

    Cast<UWPGameInstance>(GetGameInstance())->SaveChallenge();

    SetVisibility(ESlateVisibility::Hidden);

}
