// Fill out your copyright notice in the Description page of Project Settings.


#include "userWidget/mainMenu/WPKeySettingUserWidget.h"
#include <GameFramework/InputSettings.h>
#include "Components/TextBlock.h"

void UWPKeySettingUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UInputSettings* MyInputSettings = UInputSettings::GetInputSettings();

	if (bIsAxisMapping)
	{
		TArray<FInputAxisKeyMapping> AxisKeyMappings;
		MyInputSettings->GetAxisMappingByName(FName(MappingName), AxisKeyMappings);

		FString KeyString;
		for (FInputAxisKeyMapping KeyMapping : AxisKeyMappings)
			if (KeyMapping.Scale == AxisValue)
			{
				KeyString = KeyMapping.Key.ToString();
				break;
			}
		Cast<UTextBlock>(GetWidgetFromName("KeyText"))->SetText(FText::FromString(KeyString));
	}
	else 
	{
		TArray<FInputActionKeyMapping> ActionKeyMappings;
		MyInputSettings->GetActionMappingByName(FName(MappingName), ActionKeyMappings);
		Cast<UTextBlock>(GetWidgetFromName("KeyText"))->SetText(FText::FromString(ActionKeyMappings[0].Key.ToString()));
	}
	

}

FReply UWPKeySettingUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		SetKeyboardFocus();

	return FReply::Handled();
}

FReply UWPKeySettingUserWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	UInputSettings* MyInputSettings = UInputSettings::GetInputSettings();


	if (bIsAxisMapping)
	{
		TArray<FInputAxisKeyMapping> AxisKeyMappings;
		MyInputSettings->GetAxisMappingByName(FName(MappingName), AxisKeyMappings);

		int32 KeyIndex;
		for (int32 i = 0; i < AxisKeyMappings.Num(); i++)
		{
			if (AxisKeyMappings[i].Scale == AxisValue)
			{
				KeyIndex = i;
				break;
			}
		}

		if(AxisKeyMappings[1-KeyIndex].Key == InKeyEvent.GetKey())
			return Super::NativeOnKeyDown(InGeometry, InKeyEvent);

		MyInputSettings->RemoveAxisMapping(AxisKeyMappings[KeyIndex]);

		AxisKeyMappings[KeyIndex].Key = InKeyEvent.GetKey();

		MyInputSettings->AddAxisMapping(AxisKeyMappings[KeyIndex]);
		

	}
	else
	{
		TArray<FInputActionKeyMapping> ActionKeyMappings;
		MyInputSettings->GetActionMappingByName(FName(MappingName), ActionKeyMappings);

		MyInputSettings->RemoveActionMapping(ActionKeyMappings[0]);

		ActionKeyMappings[0].Key = InKeyEvent.GetKey();

		MyInputSettings->AddActionMapping(ActionKeyMappings[0]);
	}

	Cast<UTextBlock>(GetWidgetFromName("KeyText"))->SetText(FText::FromString(InKeyEvent.GetKey().ToString()));

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
