// Fill out your copyright notice in the Description page of Project Settings.


#include "player/WPPlayerController.h"
#include "userwidget/player/WPPlayerHUIUserWidget.h"
#include "player/WPPlayerActor.h"
#include <Kismet/GameplayStatics.h>

AWPPlayerController::AWPPlayerController()
{
	static ConstructorHelpers::FClassFinder<UWPPlayerHUIUserWidget> UserWidgetClassRef(TEXT("/Game/widget/player/PlayerHUDWidget.PlayerHUDWidget_C"));
	if (UserWidgetClassRef.Class)
	{
		PlayerHUIWidgetClass = UserWidgetClassRef.Class;
	}
	else 
	{
		UE_LOG(LogAssetData, Fatal, TEXT("Failed to load player HUD widget."));
	}
}

void AWPPlayerController::BeginPlay()
{
	Super::BeginPlay();


	PlayerHUIWidget = CreateWidget<UWPPlayerHUIUserWidget>(this, PlayerHUIWidgetClass);
	if (PlayerHUIWidget)
	{
		PlayerHUIWidget->AddToViewport();
		Cast<AWPPlayerActor>(GetPawn())->SetPlayerHUIUserWidget(PlayerHUIWidget);
		if (UGameplayStatics::GetCurrentLevelName(GetWorld()).Equals(TEXT("Default")))
			PlayerHUIWidget->SetLoadingScreen(true);
	}
	else 
	{
		UE_LOG(LogAssetData, Fatal, TEXT("Failed to view player HUD widget."));
	}

	SetShowMouseCursor(true);
	CurrentMouseCursor = EMouseCursor::TextEditBeam;
}
