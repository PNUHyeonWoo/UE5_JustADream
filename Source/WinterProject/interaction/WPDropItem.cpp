// Fill out your copyright notice in the Description page of Project Settings.


#include "interaction/WPDropItem.h"
#include "player/WPPlayerActor.h"
#include "dataAsset/item/WPItemDataAsset.h"

// Sets default values
AWPDropItem::AWPDropItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionProfileName(TEXT("DropItem"));
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCanEverAffectNavigation(false);
	Tags.Add(TEXT("ky_shot"));
}

void AWPDropItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWPDropItem::Interact(AWPPlayerActor* PlayerActor)
{
	if (PlayerActor->TryToReceiveItem(ItemName, Amount))
		Destroy();
}

AWPDropItem* AWPDropItem::CreateDropItem(UObject* WorldObject, FVector Location, FName ItemName, float Amount)
{
	AWPDropItem* NewDropItem = WorldObject->GetWorld()->SpawnActor<AWPDropItem>(AWPDropItem::StaticClass(), Location,FRotator());

	if (!NewDropItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Create DropItem."));
		return nullptr;
	}

	UWPItemDataAsset* ItemData = UWPItemDataAsset::GetItemData(ItemName);

	if (ItemData->ItemMesh.IsPending())
		ItemData->ItemMesh.LoadSynchronous();

	NewDropItem->MeshComponent->SetStaticMesh(ItemData->ItemMesh.Get());

	NewDropItem->ItemName = ItemName;
	NewDropItem->Amount = Amount == -1 ? ItemData->InitialCapacity : Amount;

	return NewDropItem;
}



