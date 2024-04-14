// Fill out your copyright notice in the Description page of Project Settings.


#include "userWidget/tutorial/WPMoveTutorialUserWidget.h"
#include <GameFramework/InputSettings.h>
#include "Components/TextBlock.h"

void UWPMoveTutorialUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UInputSettings* MyInputSettings = UInputSettings::GetInputSettings();

	FString TempStirng = TEXT("");

	TArray<FInputAxisKeyMapping> AxisKeyMappings;

	MyInputSettings->GetAxisMappingByName("MoveY", AxisKeyMappings);
	TempStirng += AxisKeyMappings[0].Scale == -1.0 ? AxisKeyMappings[0].Key.ToString() : AxisKeyMappings[1].Key.ToString();
	TempStirng += AxisKeyMappings[1].Scale == -1.0 ? AxisKeyMappings[0].Key.ToString() : AxisKeyMappings[1].Key.ToString();
	AxisKeyMappings.Empty();

	MyInputSettings->GetAxisMappingByName("MoveX", AxisKeyMappings);
	TempStirng += AxisKeyMappings[0].Scale == -1.0 ? AxisKeyMappings[0].Key.ToString() : AxisKeyMappings[1].Key.ToString();
	TempStirng += AxisKeyMappings[1].Scale == -1.0 ? AxisKeyMappings[0].Key.ToString() : AxisKeyMappings[1].Key.ToString();

	Cast<UTextBlock>(GetWidgetFromName("MoveYX"))->SetText(FText::FromString(TempStirng));


	TArray<FInputActionKeyMapping> ActionKeyMappings;
	MyInputSettings->GetActionMappingByName("Run",ActionKeyMappings);
	Cast<UTextBlock>(GetWidgetFromName("Run"))->SetText(FText::FromString(ActionKeyMappings[0].Key.ToString()));
	ActionKeyMappings.Empty();


	MyInputSettings->GetActionMappingByName("Swap", ActionKeyMappings);
	//TempStirng = TEXT("");
	//TempStirng += ActionKeyMappings[ActionKeyMappings.Num()-1].Key.ToString();
	//if(ActionKeyMappings.Num() > 1)
	//	TempStirng += TEXT("&") + ActionKeyMappings[0].Key.ToString();
	Cast<UTextBlock>(GetWidgetFromName("Swap"))->SetText(FText::FromString(ActionKeyMappings[0].Key.ToString()));
	ActionKeyMappings.Empty();


	MyInputSettings->GetActionMappingByName("Interaction", ActionKeyMappings);
	Cast<UTextBlock>(GetWidgetFromName("Interaction"))->SetText(FText::FromString(ActionKeyMappings[0].Key.ToString()));
	ActionKeyMappings.Empty();

	MyInputSettings->GetActionMappingByName("Map", ActionKeyMappings);
	Cast<UTextBlock>(GetWidgetFromName("Map"))->SetText(FText::FromString(ActionKeyMappings[0].Key.ToString()));
	ActionKeyMappings.Empty();
	
}
