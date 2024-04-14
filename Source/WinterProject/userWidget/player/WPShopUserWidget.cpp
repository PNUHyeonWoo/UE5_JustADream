// Fill out your copyright notice in the Description page of Project Settings.


#include "userWidget/player/WPShopUserWidget.h"
#include <Components/Button.h>
#include <player/WPPlayerActor.h>
#include "WPGameInstance.h"
#include "saveGame/WPPlayerSaveGame.h"
#include <Kismet/GameplayStatics.h>
#include <Components/TextBlock.h>

const uint32 UWPShopUserWidget::GearUpgradePrice[] = {250,500};

void UWPShopUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Cast<UButton>(GetWidgetFromName("EXITButton"))->OnClicked.AddDynamic(this, &UWPShopUserWidget::ClickExitButton);
	Cast<UButton>(GetWidgetFromName("GearUpgradeButton"))->OnClicked.AddDynamic(this, &UWPShopUserWidget::ClickGearUpgradeButton);

	UWPPlayerSaveGame* PlayerSaveGame = Cast<UWPPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(Cast<UWPGameInstance>(GetGameInstance())->PlayerSlot, 0));

	if (PlayerSaveGame->GearLevel == 2)
		Cast<UTextBlock>(GetWidgetFromName("GearUpgradeButtonText"))->SetText(FText::FromString(TEXT("Max")));
	else
		Cast<UTextBlock>(GetWidgetFromName("GearUpgradeButtonText"))->SetText(FText::FromString(FString::FromInt(GearUpgradePrice[PlayerSaveGame->GearLevel])));

}

void UWPShopUserWidget::ClickExitButton()
{
	SetVisibility(ESlateVisibility::Hidden);
	if(!Cast<AWPPlayerActor>((GetWorld()->GetFirstPlayerController()->GetPawn()))->GetIsDead())
		Cast<AWPPlayerActor>((GetWorld()->GetFirstPlayerController()->GetPawn()))->SetPlayerInput(true);
}

void UWPShopUserWidget::ClickGearUpgradeButton()
{
	AWPPlayerActor* PlayerActor = Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	const UWPPlayerSaveGame* PlayerSaveGame = PlayerActor->GetPlayerSaveGame();

	if (PlayerSaveGame->GearLevel == 2)
		return;

	if (PlayerActor->TryToSpendGold(GearUpgradePrice[PlayerSaveGame->GearLevel]))
	{
		PlayerActor->UpgradeGear();

		if (PlayerSaveGame->GearLevel == 2)
			Cast<UTextBlock>(GetWidgetFromName("GearUpgradeButtonText"))->SetText(FText::FromString(TEXT("Max")));
		else
			Cast<UTextBlock>(GetWidgetFromName("GearUpgradeButtonText"))->SetText(FText::FromString(FString::FromInt(GearUpgradePrice[PlayerSaveGame->GearLevel])));
	}

	

}

void UWPShopUserWidget::GiveMoneyToPlayer()
{
	AWPPlayerActor* PlayerActor = Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PlayerActor->ReceiveGold(1000);
}
