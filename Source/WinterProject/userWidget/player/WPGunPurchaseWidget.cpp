// Fill out your copyright notice in the Description page of Project Settings.


#include "userWidget/player/WPGunPurchaseWidget.h"
#include "dataAsset/WPGunDataAsset.h"
#include "WPGameInstance.h"
#include "saveGame/WPPlayerSaveGame.h"
#include "player/WPPlayerActor.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include <Kismet/GameplayStatics.h>

void UWPGunPurchaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PurchaseButtonWidget = Cast<UButton>(GetWidgetFromName(TEXT("GunPurchaseButton")));
	PurchaseButtonWidget->OnClicked.AddDynamic(this, &UWPGunPurchaseWidget::PurchaseButtonClick);

	FName GunName = GetFName();
	UWPGunDataAsset* GunData = UWPGunDataAsset::GetGunData(GunName);

	Cast<UImage>(GetWidgetFromName(TEXT("GunImage")))->SetBrushFromTexture(GunData->GunImage);

	FString RenderName(TEXT(""));

	FString NameString = GunName.ToString();

	for (int i = 0; i < NameString.Len(); i++)
	{
		if (isupper(NameString[i]))
		{
			RenderName += TEXT(" ");
			RenderName.AppendChar(tolower(NameString[i]));
		}
		else 
		{
			RenderName.AppendChar(NameString[i]);
		}
	}

	Cast<UTextBlock>(GetWidgetFromName(TEXT("GunNameText")))->SetText(FText::FromString(RenderName));

	UWPPlayerSaveGame* PlayerSaveGame = Cast<UWPPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(Cast<UWPGameInstance>(GetGameInstance())->PlayerSlot, 0));

	if (PlayerSaveGame->GunPurchase[GunName])
		Cast<UTextBlock>(GetWidgetFromName(TEXT("PurchaseButtonText")))->SetText(FText::FromString(FString::FromInt(GunData->AmmoPrice)));
	else
		Cast<UTextBlock>(GetWidgetFromName(TEXT("PurchaseButtonText")))->SetText(FText::FromString(FString::FromInt(GunData->Price)));

}

void UWPGunPurchaseWidget::PurchaseButtonClick()
{

	AWPPlayerActor* PlayerActor = Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	const UWPPlayerSaveGame* PlayerSaveGame = PlayerActor->GetPlayerSaveGame();

	UWPGunDataAsset* GunData = UWPGunDataAsset::GetGunData(GetFName());

	if (PlayerSaveGame->GunPurchase[GetFName()])
	{
		if (PlayerActor->TryToSpendGold(GunData->AmmoPrice))
			PlayerActor->TryToReceiveGun(GetFName());
	}
	else 
	{
		if (PlayerActor->TryToSpendGold(GunData->Price))
		{
			PlayerActor->SetGunAsPurchase(GetFName());
			Cast<UTextBlock>(GetWidgetFromName(TEXT("PurchaseButtonText")))->SetText(FText::FromString(FString::FromInt(GunData->AmmoPrice)));
			PlayerActor->TryToReceiveGun(GetFName());
		}
	}

}
