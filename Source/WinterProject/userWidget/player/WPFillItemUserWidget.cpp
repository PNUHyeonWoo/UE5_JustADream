// Fill out your copyright notice in the Description page of Project Settings.


#include "userWidget/player/WPFillItemUserWidget.h"
#include <Components/Image.h>
#include "player/WPPlayerActor.h"


FReply UWPFillItemUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn())->ClickItem(1, YIndex, XIndex);
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
		Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn())->ClickItem(0, YIndex, XIndex);

	return FReply::Handled();
}

void UWPFillItemUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetRemainMagazineAmmoVisible(YIndex, XIndex, true);
}

void UWPFillItemUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetRemainMagazineAmmoVisible(YIndex, XIndex, false);
}
