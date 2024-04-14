// Fill out your copyright notice in the Description page of Project Settings.


#include "userWidget/player/WPPlayerDeadUserWidget.h"
#include "Components/Button.h"
#include "WPGameInstance.h"
#include "player/WPPlayerActor.h"
#include <Kismet/GameplayStatics.h>
#include <Components/TextBlock.h>

void UWPPlayerDeadUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Cast<UButton>(GetWidgetFromName(TEXT("EXITButton")))->OnClicked.AddDynamic(this, &UWPPlayerDeadUserWidget::ClickEXITButton);
}

void UWPPlayerDeadUserWidget::ClickEXITButton()
{
	UWPGameInstance* WPGameInstance = Cast<UWPGameInstance>(GetWorld()->GetGameInstance());
	WPGameInstance->SetExperience(WPGameInstance->GetExperience() + Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetCumulativeEXP());
	WPGameInstance->SaveSkill();

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenuLevel"));
}

void UWPPlayerDeadUserWidget::SetEXP(float EXP)
{
	Cast<UTextBlock>(GetWidgetFromName(TEXT("EXPText")))->SetText(FText::FromString(FString::FromInt(EXP)));
}
