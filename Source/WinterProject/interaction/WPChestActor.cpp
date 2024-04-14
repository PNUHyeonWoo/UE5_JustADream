// Fill out your copyright notice in the Description page of Project Settings.


#include "interaction/WPChestActor.h"
#include "interaction/WPDropItem.h"
#include "interaction/WPDropGold.h"
#include "NiagaraComponent.h"

// Sets default values
AWPChestActor::AWPChestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Amount = 1;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	MeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	MeshComponent->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode);

	EffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EffectComponent"));
	EffectComponent->SetupAttachment(MeshComponent);

}

void AWPChestActor::Interact(AWPPlayerActor* PlayerActor)
{
	if (IsOpened)
		return;

	IsOpened = 1;

	MeshComponent->Play(false);

	float ItemHeight = 0;

	for (uint32 i = 0; i < Amount; i++)
	{
		FChestDropItem SelectedItem = ChestDropItems[FMath::RandHelper(ChestDropItems.Num())];
		for (uint32 j = 0; j < SelectedItem.Amount; j++)
		{
			AWPDropItem::CreateDropItem(GetWorld(), GetActorLocation() + GetActorUpVector() * 130 + FVector(0,0,ItemHeight), SelectedItem.ItemName);
			ItemHeight += 50;
		}
	}

	AWPDropGold::CreateDropGold(GetWorld(), FMath::RandRange(MinGold, MaxGold), GetActorLocation() + GetActorUpVector() * 130 + FVector(0, 0, ItemHeight));
	EffectComponent->Deactivate();

}

