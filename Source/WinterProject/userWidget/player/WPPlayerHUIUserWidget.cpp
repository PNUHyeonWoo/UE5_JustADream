// Fill out your copyright notice in the Description page of Project Settings.


#include "userWidget/player/WPPlayerHUIUserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include <player/WPPlayerActor.h>
#include <saveGame/WPPlayerSaveGame.h>
#include "dataAsset/WPGunDataAsset.h"
#include "dataAsset/WPAmmoDataAsset.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"
#include "userWidget/player/WPFillItemUserWidget.h"
#include "dataAsset/item/WPItemDataAsset.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "userWidget/player/WPShopUserWidget.h"
#include "dataAsset/statusEffect/WPStatusEffectDataAsset.h"
#include "userWidget/player/WPMapUserWidget.h"
#include <Kismet/GameplayStatics.h>
#include <Components/Button.h>
#include "userWidget/player/WPPlayerDeadUserWidget.h"
#include "userWidget/player/WPUnFillitemUserWidget.h"
#include <WPGameInstance.h>
#include "userWidget/player/WPTutorialExplanationUserWidget.h"

UWPPlayerHUIUserWidget::UWPPlayerHUIUserWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> AimImageRef(TEXT("/Script/Engine.Texture2D'/Game/player/mouseCursorImage/DirectZoom.DirectZoom'"));
	if (AimImageRef.Succeeded())
		NonTrueAimCursorImage = AimImageRef.Object;
	else
		UE_LOG(LogAssetData, Error, TEXT("Failed to load Aim Image."));

	static ConstructorHelpers::FObjectFinder<UTexture2D> TrueAimImageRef(TEXT("/Script/Engine.Texture2D'/Game/player/mouseCursorImage/AImZoom.AImZoom'"));
	if (TrueAimImageRef.Succeeded())
		TrueAimCursorImage = TrueAimImageRef.Object;
	else
		UE_LOG(LogAssetData, Error, TEXT("Failed to load Aim Image."));

	static ConstructorHelpers::FObjectFinder<UTexture2D> EmptyItemImageRef(TEXT("/Script/Engine.Texture2D'/Game/Images/UI/ItemAmmo/Item/EmptyBag.EmptyBag'"));
	if (EmptyItemImageRef.Succeeded())
		EmptyItemImage = EmptyItemImageRef.Object;
	else
		UE_LOG(LogAssetData, Fatal, TEXT("Failed to load empty item Image."));


	static ConstructorHelpers::FClassFinder<UUserWidget> FillItemWidgetClassRef(TEXT("/Game/widget/player/FillItemWidget.FillItemWidget_C"));
	if (FillItemWidgetClassRef.Class)
		FillItemWidgetClass = FillItemWidgetClassRef.Class;
	else
		UE_LOG(LogAssetData, Fatal, TEXT("Failed to load FillItem widget subclass."));

	static ConstructorHelpers::FClassFinder<UUserWidget> UnFillItemWidgetClassRef(TEXT("/Game/widget/player/UnFillItemWidget.UnFillItemWidget_C"));
	if (UnFillItemWidgetClassRef.Class)
		UnFillItemWidgetClass = UnFillItemWidgetClassRef.Class;
	else
		UE_LOG(LogAssetData, Fatal, TEXT("Failed to load UnFillItem widget subclass."));


}

void UWPPlayerHUIUserWidget::RefreshGear(uint8 GearPart)
{
	const float ItemImageXSize = 60;
	const float ItemImageYSize = 105;

	const UWPPlayerSaveGame* PlayerSaveGame = Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetPlayerSaveGame();

	if (GearPart < 2 && PlayerSaveGame->PossessedGuns[GearPart] == TEXT(""))
			return;


	if (GearPart < 2) // refresh ammo
	{
		UWPAmmoDataAsset* AmmoData = UWPGunDataAsset::GetGunData(PlayerSaveGame->PossessedGuns[GearPart])->AmmoUsed;
		UTexture2D* FillImage = AmmoData->AmmoImage;
		UTexture2D* NotFillImage = AmmoData->EmptyImage;

		TArray<int8> Ammos;
		if (GearPart == 0)
			Ammos = PlayerSaveGame->AmmoInGearUpper;
		else
			Ammos = PlayerSaveGame->AmmoInGearLower;
		
		UHorizontalBox* ParentPanel;
		if (GearPart == 0)
			ParentPanel = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("UpperAmmoPanel")));
		else
			ParentPanel = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("LowerAmmoPanel")));

		TArray<UWidget*> Children = ParentPanel->GetAllChildren();

		for (UWidget* Child : Children)
		{
			ParentPanel->RemoveChild(Child);
			Child->RemoveFromParent();
		}

		for (int i = 0; i < Ammos.Num(); i++)
		{
			UImage* ImageWidget;

			if (Ammos[i] < 0)
			{
				//ImageWidget = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
				//ParentPanel->AddChild(ImageWidget);
				//ImageWidget->SetBrushFromTexture(NotFillImage);

				UWPUnFillitemUserWidget* UnFillItemWidget = Cast<UWPUnFillitemUserWidget>(CreateWidget<UWPUnFillitemUserWidget>(this, UnFillItemWidgetClass));
				ParentPanel->AddChild(UnFillItemWidget);
				ImageWidget = Cast<UImage>(UnFillItemWidget->GetWidgetFromName("Image"));
				ImageWidget->SetBrushFromTexture(NotFillImage);
			}
			else 
			{
				UWPFillItemUserWidget* FillItemWidget = Cast<UWPFillItemUserWidget>(CreateWidget<UWPFillItemUserWidget>(this, FillItemWidgetClass));
				FillItemWidget->YIndex = GearPart;
				FillItemWidget->XIndex = i;
				ParentPanel->AddChild(FillItemWidget);
				ImageWidget = Cast<UImage>(FillItemWidget->GetWidgetFromName("Image"));
				ImageWidget->SetBrushFromTexture(FillImage);
			}

			float YDX = (float) FillImage->GetSizeY() / (float)FillImage->GetSizeX();

			ImageWidget->Brush.SetImageSize(FVector2D(AmmoData->ImageXSize, AmmoData->ImageXSize * YDX));

		}


	}
	else //refresh Item
	{
		TArray<UWPItemDataAsset*> Items = PlayerSaveGame->ItemsInGear;

		UHorizontalBox* ParentPanel;
		ParentPanel = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("ItemPanel")));

		TArray<UWidget*> Children = ParentPanel->GetAllChildren();

		for (UWidget* Child : Children)
		{
			ParentPanel->RemoveChild(Child);
			Child->RemoveFromParent();
		}

		for (int i = 0; i < Items.Num(); i++)
		{
			UImage* ImageWidget;

			if (Items[i] == nullptr)
			{
				ImageWidget = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
				ParentPanel->AddChild(ImageWidget);
				ImageWidget->SetBrushFromTexture(EmptyItemImage);
			}
			else
			{
				UWPFillItemUserWidget* FillItemWidget = Cast<UWPFillItemUserWidget>(CreateWidget<UWPFillItemUserWidget>(this, FillItemWidgetClass));
				FillItemWidget->YIndex = GearPart;
				FillItemWidget->XIndex = i;
				ParentPanel->AddChild(FillItemWidget);
				ImageWidget = Cast<UImage>(FillItemWidget->GetWidgetFromName("Image"));
				ImageWidget->SetBrushFromTexture(Items[i]->ItemImage);
			}

			ImageWidget->Brush.SetImageSize(FVector2D(ItemImageXSize,ItemImageYSize));

		}




	}

}

void UWPPlayerHUIUserWidget::OpenShop()
{
	ShopWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWPPlayerHUIUserWidget::HideShop()
{
	ShopWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UWPPlayerHUIUserWidget::CreateStatusEffectImages(TMap<FName, float> StatusEffects)
{
	UHorizontalBox* StatusEffectBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("StatusEffectBox")));

	for (TTuple<FName, float> StatusEffect : StatusEffects)
	{
		UImage* ImageWidget = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(),StatusEffect.Key);
		StatusEffectBox->AddChild(ImageWidget);
		ImageWidget->SetBrushFromTexture(UWPStatusEffectDataAsset::GetStatusEffectData(StatusEffect.Key)->StatusEffectIcon);
	}
}

void UWPPlayerHUIUserWidget::SetStatusEffectImageVisiblity(FName StatusEffectName, bool Value)
{
	const float ImageSize = 32;

	UImage* ImageWidget = Cast<UImage>(GetWidgetFromName(StatusEffectName));
	ImageWidget->Brush.SetImageSize(FVector2D(Value ? ImageSize : 0, ImageSize));
}

void UWPPlayerHUIUserWidget::OpenPlayerDeadWidget()
{
	GetWidgetFromName(TEXT("PlayerDeadWidget"))->SetVisibility(ESlateVisibility::Visible);
}

void UWPPlayerHUIUserWidget::GenerateRoomForMap(TArray<class UWPArena*> Arenas)
{
	Cast<UWPMapUserWidget>(GetWidgetFromName(TEXT("MapWidget")))->GenerateRoom(Arenas);
}

void UWPPlayerHUIUserWidget::GeneratePathForMap(TArray<FVector2D> Paths)
{
	Cast<UWPMapUserWidget>(GetWidgetFromName(TEXT("MapWidget")))->GeneratePath(Paths);
}

void UWPPlayerHUIUserWidget::ToggleMap()
{
	UWidget* MapWidget = GetWidgetFromName(TEXT("MapWidget"));

	if (MapWidget->Visibility == ESlateVisibility::Hidden)
		MapWidget->SetVisibility(ESlateVisibility::Visible);
	else
		MapWidget->SetVisibility(ESlateVisibility::Hidden);

}

void UWPPlayerHUIUserWidget::ToggleMainMenuPanel()
{
	UWidget* PanelWidget = GetWidgetFromName(TEXT("MainMenuPanel"));

	if (PanelWidget->Visibility == ESlateVisibility::Hidden)
		PanelWidget->SetVisibility(ESlateVisibility::Visible);
	else
		PanelWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UWPPlayerHUIUserWidget::SetEXPText(float EXP)
{
	Cast<UWPPlayerDeadUserWidget>(GetWidgetFromName(TEXT("PlayerDeadWidget")))->SetEXP(EXP);
}

void UWPPlayerHUIUserWidget::SetStaminaBarCanRun(bool Value)
{
	if(Value)
		StaminaBarWidget->SetFillColorAndOpacity(FLinearColor(0.125281, 1, 0.10048, 1));
	else
		StaminaBarWidget->SetFillColorAndOpacity(FLinearColor(0.039803, 0.317708, 0.031923, 1));
}

void UWPPlayerHUIUserWidget::SetLoadingScreen(bool Value)
{
	if (Value)
	{
		GetWidgetFromName(TEXT("LoadingCanvas"))->SetVisibility(ESlateVisibility::Visible);
		Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetPlayerInputToFalsePerpect();
	}
	else
	{
		GetWidgetFromName(TEXT("LoadingCanvas"))->SetVisibility(ESlateVisibility::HitTestInvisible);
		GetWidgetFromName(TEXT("LoadingText"))->SetVisibility(ESlateVisibility::Hidden);
		bIsFadeIn = 1;
		Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetPlayerInputToTruePerpect();
	}

}

void UWPPlayerHUIUserWidget::SetRemainAmmoVisible(float RemainAmmo)
{
	if (RemainAmmo >= 0)
	{
		Cast<UProgressBar>(GetWidgetFromName(TEXT("RemainAmmoProgressBar")))->SetPercent(RemainAmmo);
		GetWidgetFromName(TEXT("RemainAmmoCanvas"))->SetVisibility(ESlateVisibility::Visible);
	}
	else
		GetWidgetFromName(TEXT("RemainAmmoCanvas"))->SetVisibility(ESlateVisibility::Hidden);

}

void UWPPlayerHUIUserWidget::SetRemainMagazineAmmoVisible(float RemainAmmo)
{
	if (RemainAmmo >= 0)
	{
		Cast<UProgressBar>(GetWidgetFromName(TEXT("RemainMagazineAmmoProgressBar")))->SetPercent(RemainAmmo);
		GetWidgetFromName(TEXT("RemainMagazineAmmoCanvas"))->SetVisibility(ESlateVisibility::Visible);
	}
	else
		GetWidgetFromName(TEXT("RemainMagazineAmmoCanvas"))->SetVisibility(ESlateVisibility::Hidden);
}

void UWPPlayerHUIUserWidget::LiftPathFog(FVector2D PathIndex)
{
	Cast<UWPMapUserWidget>(GetWidgetFromName(TEXT("MapWidget")))->LiftPathFog(PathIndex);
}

void UWPPlayerHUIUserWidget::SetBossIcon(uint8 Index)
{
	Cast<UWPMapUserWidget>(GetWidgetFromName(TEXT("MapWidget")))->SetBossIcon(Index);
}

void UWPPlayerHUIUserWidget::SetTutorialWidget(FString Id)
{
	if (Id == TEXT(""))
	{
		GetWidgetFromName(TEXT("TutorialExplanationWidget"))->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	Cast<UWPTutorialExplanationUserWidget>(GetWidgetFromName(TEXT("TutorialExplanationWidget")))->SetNewStirngTableId(Id);
	GetWidgetFromName(TEXT("TutorialExplanationWidget"))->SetVisibility(ESlateVisibility::Visible);

}

void UWPPlayerHUIUserWidget::OnClickMainMenuButton()
{
	UWPGameInstance* WPGameInstance = Cast<UWPGameInstance>(GetWorld()->GetGameInstance());
	WPGameInstance->SetExperience(WPGameInstance->GetExperience() + Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetCumulativeEXP());
	WPGameInstance->SaveSkill();

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenuLevel"));
}

void UWPPlayerHUIUserWidget::OnClickMainMenuEXITButton()
{
	GetWidgetFromName(TEXT("MainMenuPanel"))->SetVisibility(ESlateVisibility::Hidden);
}

void UWPPlayerHUIUserWidget::OnClickOptionButton() 
{
	GetWidgetFromName(TEXT("OptionWIdget"))->SetVisibility(ESlateVisibility::Visible);
}

void UWPPlayerHUIUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CursorPanelWidget = Cast<UCanvasPanel>(GetWidgetFromName("AimCursorPanel"));
	CursorImageWidget = Cast<UImage>(GetWidgetFromName("AimCursorImage"));
	HpBarWidget = Cast<UProgressBar>(GetWidgetFromName("HPBar"));
	StaminaBarWidget = Cast<UProgressBar>(GetWidgetFromName("StaminaBar"));
	GoldTextBlock = Cast<UTextBlock>(GetWidgetFromName("GoldText"));
	ShopWidget = Cast<UWPShopUserWidget>(GetWidgetFromName("ShopWidget"));
	LoadingImageWidget = Cast<UImage>(GetWidgetFromName("LoadingImage"));

	Cast<UButton>(GetWidgetFromName(TEXT("MainMenuButton")))->OnClicked.AddDynamic(this, &UWPPlayerHUIUserWidget::OnClickMainMenuButton);
	Cast<UButton>(GetWidgetFromName(TEXT("MainMenuEXITButton")))->OnClicked.AddDynamic(this, &UWPPlayerHUIUserWidget::OnClickMainMenuEXITButton);
	Cast<UButton>(GetWidgetFromName(TEXT("OptionButton")))->OnClicked.AddDynamic(this, &UWPPlayerHUIUserWidget::OnClickOptionButton);
}

void UWPPlayerHUIUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const float TrueAimImageLength = 16;
	const float AimPanelMinLength = 48;
	const float AimPanelMaxLength = 384;

	//mouse cursor traking

	FVector2D MousePosition = {0,0};

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if(PlayerController)
		PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);

	FVector2D ViewportSize;
	GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize);

	MousePosition = { MousePosition.X / ViewportSize.X, MousePosition.Y / ViewportSize.Y };

	FVector2D PanelSize = MyGeometry.GetLocalSize();

	MousePosition = { MousePosition.X * PanelSize.X,MousePosition.Y * PanelSize.Y };

	Cast<UCanvasPanelSlot>(CursorPanelWidget->Slot)->SetPosition(MousePosition);

	//mouse cursor image set

	if (Cast<AWPPlayerActor>(PlayerController->GetPawn())->GetIsTrueAiming())
	{
		CursorImageWidget->SetBrushFromTexture(TrueAimCursorImage);
		CursorImageWidget->Brush.SetImageSize(FVector2D(TrueAimImageLength, TrueAimImageLength));
		Cast<UCanvasPanelSlot>(CursorPanelWidget->Slot)->SetSize(FVector2D(TrueAimImageLength, TrueAimImageLength));
		CursorPanelWidget->SetRenderTransformAngle(0);
	}
	else 
	{
		CursorImageWidget->SetBrushFromTexture(NonTrueAimCursorImage);
		CursorImageWidget->Brush.SetImageSize(FVector2D(AimPanelMaxLength, AimPanelMaxLength));

		MousePosition = MousePosition - PanelSize / 2;

		float Angle = FMath::RadiansToDegrees(FMath::Atan2(MousePosition.Y, MousePosition.X))+90;
		CursorPanelWidget->SetRenderTransformAngle(Angle);

		MousePosition = MousePosition / PanelSize;
		MousePosition.X *= 1920.0f / 1080.0f;
		float PanelXLength  = AimPanelMinLength + (MousePosition.Length() / 1.025f) * (AimPanelMaxLength - AimPanelMinLength);

		Cast<UCanvasPanelSlot>(CursorPanelWidget->Slot)->SetSize(FVector2D(PanelXLength, AimPanelMaxLength));
	
	}

	//Stat Bar Tick

	AWPPlayerActor* PlayerActor = Cast<AWPPlayerActor>(PlayerController->GetPawn());

	if (PlayerActor->GetPlayerSaveGame())
	{
		HpBarWidget->SetPercent(PlayerActor->GetCurrentHP() / PlayerActor->GetMaxHP());

		StaminaBarWidget->SetPercent(PlayerActor->GetPlayerSaveGame()->CurrentStamina / PlayerActor->GetPlayerSaveGame()->MaxStamina);

		SetGold(PlayerActor->GetPlayerSaveGame()->Gold);
	}

	if (bIsFadeIn)
	{
		if (LoadingImageWidget->ColorAndOpacity.A > 0)
		{
			const float FadeInSpeed = 0.5;

			LoadingImageWidget->SetOpacity(LoadingImageWidget->ColorAndOpacity.A - FadeInSpeed * InDeltaTime);

			if(LoadingImageWidget->ColorAndOpacity.A <= 0)
				GetWidgetFromName(TEXT("LoadingCanvas"))->SetVisibility(ESlateVisibility::Hidden);


		}
	}


}

void UWPPlayerHUIUserWidget::SetGold(uint32 Value)
{
	GoldTextBlock->SetText(FText::FromString(FString::FromInt(Value)));
}


