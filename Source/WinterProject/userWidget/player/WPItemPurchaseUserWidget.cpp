// Fill out your copyright notice in the Description page of Project Settings.


#include "userWidget/player/WPItemPurchaseUserWidget.h"
#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <dataAsset/item/WPItemDataAsset.h>
#include "player/WPPlayerActor.h"

void UWPItemPurchaseUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Cast<UButton>(GetWidgetFromName(TEXT("ItemPurchaseButton")))->OnClicked.AddDynamic(this, &UWPItemPurchaseUserWidget::PurchaseButtonClick);

	UWPItemDataAsset* ItemData = UWPItemDataAsset::GetItemData(GetFName());

	Cast<UImage>(GetWidgetFromName(TEXT("ItemImage")))->SetBrushFromTexture(ItemData->ItemShopImage);
	Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNameText")))->SetText(FText::FromName(GetFName()));
	Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemPurchaseButtonText")))->SetText(FText::FromString(FString::FromInt(ItemData->Price)));

}

void UWPItemPurchaseUserWidget::PurchaseButtonClick()
{
	AWPPlayerActor* PlayerActor = Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if (PlayerActor->GetEmptyItemSpace() != -1)
	{
		UWPItemDataAsset* ItemData = UWPItemDataAsset::GetItemData(GetFName());
		if (PlayerActor->TryToSpendGold(ItemData->Price))
			PlayerActor->TryToReceiveItem(GetFName());
	}
}
