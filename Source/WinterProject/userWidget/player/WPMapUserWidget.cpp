// Fill out your copyright notice in the Description page of Project Settings.


#include "userWidget/player/WPMapUserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "generateMap/WPArena.h"
#include "Blueprint/WidgetTree.h"
#include "Components/WrapBox.h"

UWPMapUserWidget::UWPMapUserWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	for (int y = 0; y < 5; y++)
	{
		RoomArray.Add(TArray<TArray<uint8>>());
		for (int x = 0; x < 5; x++)
		{
			RoomArray[y].Add(TArray<uint8>());
		}
	}
}

void UWPMapUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Cast<UButton>(GetWidgetFromName(TEXT("EXITButton")))->OnClicked.AddDynamic(this, &UWPMapUserWidget::OnClickEXITButton);

	UCanvasPanel* MapCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("MapCanvas")));

	const float PathLength = 30;
	float CellLength = (Cast<UCanvasPanelSlot>(MapCanvas->Slot)->GetSize().X - 4 * PathLength) / 5;

	float FogLength = PathLength / 2 + CellLength;

	for(int y = 0;y<5;y++)
		for (int x = 0; x < 5; x++)
		{
			UImage* ImageWidget = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(),FName(FString("F") + FString::FromInt(y) + FString("_") + FString::FromInt(x)));
			MapCanvas->AddChild(ImageWidget);
			UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(ImageWidget->Slot);
			PanelSlot->SetAnchors(FAnchors(1, 0, 1, 0));
			PanelSlot->SetAlignment(FVector2D(1, 0));

			//ImageWidget->SetColorAndOpacity(FLinearColor(0.693872, 0.693872, 0.693872,1));
			ImageWidget->SetColorAndOpacity(FLinearColor(0.1, 0.1, 0.1, 1));

			float XPosition = -x * FogLength;
			float YPosition = y * FogLength;

			if (x >= 2)
				XPosition -= PathLength / 2 * (x - 1);
			if (y >= 2)
				YPosition += PathLength / 2 * (y - 1);


			PanelSlot->SetPosition(FVector2D(XPosition, YPosition));

			float XSize = FogLength;
			float YSize = FogLength;

			if (1 <= x && x <= 3)
				XSize += PathLength / 2;
			if (1 <= y && y <= 3)
				YSize += PathLength / 2;

			PanelSlot->SetSize(FVector2D(XSize, YSize));
			PanelSlot->SetZOrder(1);

			

		}

	PlayerIconSlot = Cast<UCanvasPanelSlot>(GetWidgetFromName(TEXT("PlayerIcon"))->Slot);
	PlayerActor = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UWPMapUserWidget::OnClickEXITButton()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UWPMapUserWidget::LiftFog(uint8 YIndex, uint8 XIndex)
{
	TArray<uint8> IncludedRooms = RoomArray[YIndex][XIndex];

	for (int32 i = 0; i < IncludedRooms.Num(); i += 2)
	{
		GetWidgetFromName(FName(FString("F") + FString::FromInt(IncludedRooms[i]) + FString("_") + FString::FromInt(IncludedRooms[i + 1])))-> SetVisibility(ESlateVisibility::Hidden);
	}

}

void UWPMapUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector PlayerLocation = PlayerActor->GetActorLocation();

	const float MapRatio = 0.0470588235f;

	PlayerLocation.X = PlayerLocation.X * MapRatio + 994;
	PlayerLocation.Y = PlayerLocation.Y * MapRatio - 944;

	if (PlayerLocation.X > 990)
		PlayerLocation.X = 990;

	if (PlayerLocation.Y < -850)
		PlayerLocation.Y = -850;

	PlayerIconSlot->SetPosition(FVector2D(PlayerLocation.X, PlayerLocation.Y));

}

void UWPMapUserWidget::GenerateRoom(TArray<class UWPArena*> Arenas)
{
	UCanvasPanel* MapCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("MapCanvas")));

	const float PathLength = 30;

	float CellLength = (Cast<UCanvasPanelSlot>(MapCanvas->Slot)->GetSize().X - 4 * PathLength) / 5;


	float IndexLength = PathLength + CellLength;
	float DoubleCellLength = IndexLength + CellLength;

	for (UWPArena* Arena : Arenas)
	{
		TArray<uint8> IncludedRooms = {(uint8)(Arena->ArenaPosition.Y - 1),(uint8)(Arena->ArenaPosition.X - 1) };

		if (Arena->ArenaType == EArenaType::Arena1)
		{
			RoomArray[IncludedRooms[0]][IncludedRooms[1]] = IncludedRooms;
		}
		else if (Arena->ArenaType == EArenaType::ArenaVertical2)
		{
			IncludedRooms.Add((uint8)(Arena->ArenaPosition.Y - 1));
			IncludedRooms.Add((uint8)(Arena->ArenaPosition.X - 1) + 1);

			RoomArray[IncludedRooms[0]][IncludedRooms[1]] = IncludedRooms;
			RoomArray[IncludedRooms[0]][IncludedRooms[1]+1] = IncludedRooms;

		}
		else if (Arena->ArenaType == EArenaType::ArenaHorizontal2)
		{
			IncludedRooms.Add((uint8)(Arena->ArenaPosition.Y - 1) + 1);
			IncludedRooms.Add((uint8)(Arena->ArenaPosition.X - 1));

			RoomArray[IncludedRooms[0]][IncludedRooms[1]] = IncludedRooms;
			RoomArray[IncludedRooms[0] + 1][IncludedRooms[1]] = IncludedRooms;
		}
		else
		{
			IncludedRooms.Add((uint8)(Arena->ArenaPosition.Y - 1));
			IncludedRooms.Add((uint8)(Arena->ArenaPosition.X - 1) + 1);
			IncludedRooms.Add((uint8)(Arena->ArenaPosition.Y - 1) + 1);
			IncludedRooms.Add((uint8)(Arena->ArenaPosition.X - 1));
			IncludedRooms.Add((uint8)(Arena->ArenaPosition.Y - 1) + 1);
			IncludedRooms.Add((uint8)(Arena->ArenaPosition.X - 1) + 1);

			RoomArray[IncludedRooms[0]][IncludedRooms[1]] = IncludedRooms;
			RoomArray[IncludedRooms[0]][IncludedRooms[1] + 1] = IncludedRooms;
			RoomArray[IncludedRooms[0]+ 1][IncludedRooms[1]] = IncludedRooms;
			RoomArray[IncludedRooms[0]+ 1][IncludedRooms[1] + 1] = IncludedRooms;
		}


		UImage* ImageWidget = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		MapCanvas->AddChild(ImageWidget);
		UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(ImageWidget->Slot);
		PanelSlot->SetAnchors(FAnchors(1, 0, 1, 0));
		PanelSlot->SetAlignment(FVector2D(1, 0));
		
		PanelSlot->SetPosition(FVector2D(-(Arena->ArenaPosition.X - 1) * IndexLength, (Arena->ArenaPosition.Y - 1) * IndexLength));

		if (Arena->ArenaType == EArenaType::Arena1)
			PanelSlot->SetSize(FVector2D(CellLength, CellLength));
		else if(Arena->ArenaType == EArenaType::ArenaVertical2)
			PanelSlot->SetSize(FVector2D(DoubleCellLength, CellLength));
		else if (Arena->ArenaType == EArenaType::ArenaHorizontal2)
			PanelSlot->SetSize(FVector2D(CellLength, DoubleCellLength));
		else
			PanelSlot->SetSize(FVector2D(DoubleCellLength, DoubleCellLength));

		ImageWidget->Brush.TintColor = FLinearColor(0.3, 0.3, 0.3);

		FVector2D CenterPostion = PanelSlot->GetPosition() + FVector2D(-PanelSlot->GetSize().X / 2, PanelSlot->GetSize().Y / 2 );
		UWrapBox* WrapBox = WidgetTree->ConstructWidget<UWrapBox>(UWrapBox::StaticClass());
		MapCanvas->AddChild(WrapBox);

		UCanvasPanelSlot* WrapBoxPanelSlot = Cast<UCanvasPanelSlot>(WrapBox->Slot);

		WrapBoxPanelSlot->SetAnchors(FAnchors(1, 0, 1, 0));
		WrapBoxPanelSlot->SetAlignment(FVector2D(0.5, 0));
		WrapBoxPanelSlot->SetSize(FVector2D(PanelSlot->GetSize().X, 20));

		WrapBoxPanelSlot->SetPosition(CenterPostion + FVector2D(0, -15));
		WrapBox->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);

		for (TPair<uint32, uint32> IconPair : Arena->NumOfIcons)
		{
			UCanvasPanel* ImageCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
			WrapBox->AddChild(ImageCanvas);

			const float IconBasicSize = 20;
			const float IconIncrease = 10;


			UImage* IconImageWidget = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
			ImageCanvas->AddChild(IconImageWidget);
			UCanvasPanelSlot* IconPanelSlot = Cast<UCanvasPanelSlot>(IconImageWidget->Slot);
			IconPanelSlot->SetAnchors(FAnchors(0.5, 0.5));
			IconPanelSlot->SetAlignment(FVector2D(0.5, 0.5));
			IconPanelSlot->SetPosition(FVector2D(0, 0));

			IconImageWidget->SetBrushFromTexture(IconArray[IconPair.Key]);

			float IconSize = IconBasicSize + FMath::Sqrt((float)IconPair.Value) * IconIncrease;

			IconPanelSlot->SetSize(FVector2D(IconSize, IconSize));

		}


	}

	LiftFog(4, 4);

	GetWidgetFromName(TEXT("TutorialMap"))->SetVisibility(ESlateVisibility::Hidden);
	GetWidgetFromName(TEXT("StartRoomImage"))->SetVisibility(ESlateVisibility::Visible);
	GetWidgetFromName(TEXT("BossPathImage"))->SetVisibility(ESlateVisibility::Visible);
	GetWidgetFromName(TEXT("BossRoomImage"))->SetVisibility(ESlateVisibility::Visible);
	GetWidgetFromName(TEXT("BossIconImage"))->SetVisibility(ESlateVisibility::Visible);

}

void UWPMapUserWidget::GeneratePath(TArray<FVector2D> Paths)
{
	UCanvasPanel* MapCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("MapCanvas")));
	const float PathLength = 30;

	float CellLength = (Cast<UCanvasPanelSlot>(MapCanvas->Slot)->GetSize().X - 4 * PathLength) / 5;

	float IndexLength = PathLength + CellLength;

	for (FVector2D Path : Paths)
	{
		UImage* ImageWidget = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		MapCanvas->AddChild(ImageWidget);
		UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(ImageWidget->Slot);
		PanelSlot->SetAnchors(FAnchors(1, 0, 1, 0));
		PanelSlot->SetAlignment(FVector2D(1, 0));
		PanelSlot->SetSize(FVector2D(PathLength, PathLength));

		ImageWidget->Brush.TintColor = FLinearColor(0.3, 0.3, 0.3);

		float CanvasPosition[] = { Path.X, Path.Y };

		for (int i = 0; i < 2; i++)
		{
			if (CanvasPosition[i] - floor(CanvasPosition[i]) == 0.5)
				CanvasPosition[i] = CellLength + (CanvasPosition[i] - 1.5) * IndexLength;
			else
				CanvasPosition[i] = (CellLength - PathLength) / 2 + (CanvasPosition[i] - 1) * IndexLength;
		}

		PanelSlot->SetPosition(FVector2D(-CanvasPosition[0],CanvasPosition[1]));
	}

}

void UWPMapUserWidget::LiftPathFog(FVector2D PathIndex)
{
	if (PathIndex[0] - floor(PathIndex[0]) == 0.5) 
	{
		uint8 YIndex = PathIndex[1] - 1;
		uint8 XIndex = PathIndex[0] - 1;

		LiftFog(YIndex, XIndex);
		LiftFog(YIndex, XIndex+1);
	}
	else 
	{
		uint8 YIndex = PathIndex[1] - 1;
		uint8 XIndex = PathIndex[0] - 1;

		LiftFog(YIndex, XIndex);
		LiftFog(YIndex+1, XIndex);
	}

}

void UWPMapUserWidget::SetBossIcon(uint8 Index)
{
	Cast<UImage>(GetWidgetFromName(TEXT("BossIconImage")))->SetBrushFromTexture(IconArray[Index]);
}
