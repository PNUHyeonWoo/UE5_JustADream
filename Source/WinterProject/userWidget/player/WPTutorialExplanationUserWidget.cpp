// Fill out your copyright notice in the Description page of Project Settings.


#include "userWidget/player/WPTutorialExplanationUserWidget.h"
#include "Internationalization/StringTableRegistry.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UWPTutorialExplanationUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlockWidget = Cast<UTextBlock>(GetWidgetFromName(TEXT("ExplanationText")));
	Cast<UButton>(GetWidgetFromName(TEXT("NextButton")))->OnClicked.AddDynamic(this, &UWPTutorialExplanationUserWidget::Next);
	Cast<UButton>(GetWidgetFromName(TEXT("PrevButton")))->OnClicked.AddDynamic(this, &UWPTutorialExplanationUserWidget::Prev);
}

void UWPTutorialExplanationUserWidget::Next()
{
	if (bIsLast)
		return;

	nowIndex++;

	FText NextText = FText::FromStringTable(*NowStringTableId,FString::FromInt(nowIndex));

	if (NextText.ToString()[0] == TCHAR('<'))
	{
		bIsLast = 1;
		TextBlockWidget->SetText(FText::FromStringTable(*LastStringTableId, TEXT("0")));
		return;
	}

	TextBlockWidget->SetText(NextText);
	bIsLast = 0;
}

void UWPTutorialExplanationUserWidget::Prev()
{
	if (nowIndex <= 0)
		return;

	bIsLast = 0;
	nowIndex--;

	FText NextText = FText::FromStringTable(*NowStringTableId, FString::FromInt(nowIndex));

	TextBlockWidget->SetText(NextText);
}

void UWPTutorialExplanationUserWidget::SetNewStirngTableId(FString Id)
{
	if (Id == NowStringTableId)
		return;

	NowStringTableId = Id;
	nowIndex = 0;
	bIsLast = 0;

	FText NextText = FText::FromStringTable(*NowStringTableId, FString::FromInt(nowIndex));

	TextBlockWidget->SetText(NextText);
}

