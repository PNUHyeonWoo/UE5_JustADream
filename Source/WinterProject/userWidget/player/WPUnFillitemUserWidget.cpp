// Fill out your copyright notice in the Description page of Project Settings.


#include "userWidget/player/WPUnFillitemUserWidget.h"
#include "player/WPPlayerActor.h"

void UWPUnFillitemUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetRemainMagazineAmmoVisible(0, 0, false);
}
