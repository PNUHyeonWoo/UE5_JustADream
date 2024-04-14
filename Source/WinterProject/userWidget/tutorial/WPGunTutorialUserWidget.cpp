// Fill out your copyright notice in the Description page of Project Settings.


#include "userWidget/tutorial/WPGunTutorialUserWidget.h"
#include "Components/TextBlock.h"
#include <GameFramework/InputSettings.h>
#include "player/WPPlayerActor.h"
#include "dataAsset/WPGunDataAsset.h"
#include <Components/CanvasPanel.h>
#include "Components/CanvasPanelSlot.h"

void UWPGunTutorialUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	NormalRealodWidget = GetWidgetFromName("NormalReload");
	NormalRealodTitleSlot = Cast<UCanvasPanelSlot>(GetWidgetFromName("NormalReloadTitleText")->Slot);
	NormalRealodSlot = Cast<UCanvasPanelSlot>(GetWidgetFromName("NormalReloadText")->Slot);

	FastRealodWidget = GetWidgetFromName("FastReload");
	FastRealodTitleSlot = Cast<UCanvasPanelSlot>(GetWidgetFromName("FastReloadTitleText")->Slot);
	FastRealodSlot = Cast<UCanvasPanelSlot>(GetWidgetFromName("FastReloadText")->Slot);

	ChamberRealodWidget = GetWidgetFromName("ChamberReload");
	ChamberRealodTitleSlot = Cast<UCanvasPanelSlot>(GetWidgetFromName("ChamberReloadTitleText")->Slot);
	ChamberRealodSlot = Cast<UCanvasPanelSlot>(GetWidgetFromName("ChamberReloadText")->Slot);

	PullBoltWidget = GetWidgetFromName("PullBolt");
	PullBoltTitleSlot = Cast<UCanvasPanelSlot>(GetWidgetFromName("PullBoltTitleText")->Slot);
	PullBoltSlot = Cast<UCanvasPanelSlot>(GetWidgetFromName("PullBoltText")->Slot);

	RackBoltWidget = GetWidgetFromName("RackBolt");
	RackBoltTitleSlot = Cast<UCanvasPanelSlot>(GetWidgetFromName("RackBoltTitleText")->Slot);
	RackBoltSlot = Cast<UCanvasPanelSlot>(GetWidgetFromName("RackBoltText")->Slot);

	UInputSettings* MyInputSettings = UInputSettings::GetInputSettings();

	TArray<FInputActionKeyMapping> ActionKeyMappings;
	MyInputSettings->GetActionMappingByName("Bolt", ActionKeyMappings);
	FString BoltKeyString = ActionKeyMappings[0].Key.ToString();

	FString ChamberReloadString = Cast<UTextBlock>(GetWidgetFromName("ChamberReloadText"))->Text.ToString();
	ChamberReloadString = ChamberReloadString.Replace(TEXT("$bolt"), *BoltKeyString);
	Cast<UTextBlock>(GetWidgetFromName("ChamberReloadText"))->SetText(FText::FromString(ChamberReloadString));

	FString PullBoltString = Cast<UTextBlock>(GetWidgetFromName("PullBoltText"))->Text.ToString();
	PullBoltString = PullBoltString.Replace(TEXT("$bolt"), *BoltKeyString);
	Cast<UTextBlock>(GetWidgetFromName("PullBoltText"))->SetText(FText::FromString(PullBoltString));

}

void UWPGunTutorialUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const FVector2D TitleSize = { 100,30 };
	const FVector2D articleSize = { 100,30 };
	const FVector2D RackBoltArticleSize = { 400,80 };

	AWPPlayerActor* PlayerActor = Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	UWPGunDataAsset* GunData = PlayerActor->GetEquippedGunData();
	if (GunData)
	{
		NormalRealodWidget->SetVisibility(GunData->CanNormalReload ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		if (GunData->CanNormalReload)
		{
			NormalRealodTitleSlot->SetSize(TitleSize);
			NormalRealodSlot->SetSize(articleSize);
			NormalRealodSlot->SetPosition(FVector2D(0, TitleSize.Y));
		}
		else
		{
			NormalRealodTitleSlot->SetSize(FVector2D(TitleSize.X,0));
			NormalRealodSlot->SetSize(FVector2D(articleSize.X, 0));
			NormalRealodSlot->SetPosition(FVector2D(0, 0));
		}

		FastRealodWidget->SetVisibility(GunData->CanFastReload ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		if (GunData->CanFastReload)
		{
			FastRealodTitleSlot->SetSize(TitleSize);
			FastRealodSlot->SetSize(articleSize);
			FastRealodSlot->SetPosition(FVector2D(0, TitleSize.Y));
		}
		else
		{
			FastRealodTitleSlot->SetSize(FVector2D(TitleSize.X, 0));
			FastRealodSlot->SetSize(FVector2D(articleSize.X, 0));
			FastRealodSlot->SetPosition(FVector2D(0, 0));
		}

		ChamberRealodWidget->SetVisibility(GunData->CanChamberReload ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		if (GunData->CanChamberReload)
		{
			ChamberRealodTitleSlot->SetSize(TitleSize);
			ChamberRealodSlot->SetSize(articleSize);
			ChamberRealodSlot->SetPosition(FVector2D(0, TitleSize.Y));
		}
		else
		{
			ChamberRealodTitleSlot->SetSize(FVector2D(TitleSize.X, 0));
			ChamberRealodSlot->SetSize(FVector2D(articleSize.X, 0));
			ChamberRealodSlot->SetPosition(FVector2D(0, 0));
		}

		PullBoltWidget->SetVisibility(GunData->MagazineCapacity > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		if (GunData->MagazineCapacity > 0)
		{
			PullBoltTitleSlot->SetSize(TitleSize);
			PullBoltSlot->SetSize(articleSize);
			PullBoltSlot->SetPosition(FVector2D(0, TitleSize.Y));
		}
		else
		{
			PullBoltTitleSlot->SetSize(FVector2D(TitleSize.X, 0));
			PullBoltSlot->SetSize(FVector2D(articleSize.X, 0));
			PullBoltSlot->SetPosition(FVector2D(0, 0));
		}

		RackBoltWidget->SetVisibility(GunData->CanRackBolt ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	}
}
