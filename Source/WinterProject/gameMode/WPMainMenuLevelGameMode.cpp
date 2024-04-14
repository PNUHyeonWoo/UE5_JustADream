// Fill out your copyright notice in the Description page of Project Settings.


#include "gameMode/WPMainMenuLevelGameMode.h"
#include "userWidget/mainMenu/WPMainMenuUserWidget.h"

AWPMainMenuLevelGameMode::AWPMainMenuLevelGameMode()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetClassRef(TEXT("/Game/widget/mainMenu/MainMenuWidget.MainMenuWidget_C"));
	if (MainMenuWidgetClassRef.Class)
		MainMenuWidgetClass = MainMenuWidgetClassRef.Class;
	else
		UE_LOG(LogAssetData, Fatal, TEXT("Failed to load main menu widget subclass."));

	DefaultPawnClass = APawn::StaticClass();
}

void AWPMainMenuLevelGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWPMainMenuUserWidget* MainMenuWidget = CreateWidget<UWPMainMenuUserWidget>(GetWorld()->GetFirstPlayerController(), MainMenuWidgetClass);
	if (MainMenuWidget)
		MainMenuWidget->AddToViewport();
	else
		UE_LOG(LogAssetData, Fatal, TEXT("Failed to create main menu widget"));

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}
