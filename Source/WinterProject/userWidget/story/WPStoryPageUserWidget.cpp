// Fill out your copyright notice in the Description page of Project Settings.


#include "userWidget/story/WPStoryPageUserWidget.h"
#include "Components/Image.h"
#include <Components/Button.h>
#include <Kismet/GameplayStatics.h>

void UWPStoryPageUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ImageWidget = Cast<UImage>(GetWidgetFromName(TEXT("StoryImage")));
	if(Images.Num() >= 1)
		ImageWidget->SetBrushFromTexture(Images[0]);
	Cast<UButton>(GetWidgetFromName(TEXT("SkipButton")))->OnClicked.AddDynamic(this,&UWPStoryPageUserWidget::OnClickSkipButton);

	UGameplayStatics::PlaySoundAtLocation(this, Sounds[0], FVector(0, 0, 0));
}

void UWPStoryPageUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const float FadeSpeed = 2;

	float NowA = ImageWidget->ColorAndOpacity.A;
	if (NowPage < TargetPage)
	{
		ImageWidget->SetOpacity(NowA - FadeSpeed * InDeltaTime);
		if (ImageWidget->ColorAndOpacity.A <= 0)
		{
			ImageWidget->SetBrushFromTexture(Images[TargetPage]);
			NowPage = TargetPage;

			if (TargetPage < Sounds.Num())
				UGameplayStatics::PlaySoundAtLocation(this, Sounds[TargetPage], FVector(0, 0, 0));
		}
	}
	else if (NowPage == TargetPage)
	{
		if (NowA < 1)
		{
			ImageWidget->SetOpacity(NowA + FadeSpeed * InDeltaTime);
		}
	}
}

void UWPStoryPageUserWidget::OnClickSkipButton()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenuLevel"));
}

void UWPStoryPageUserWidget::OnClickImage()
{
	if (ImageWidget->ColorAndOpacity.A >= 1 && NowPage == TargetPage)
	{
		if (TargetPage >= Images.Num() - 1)
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenuLevel"));
		else
			TargetPage++;

	}
}
