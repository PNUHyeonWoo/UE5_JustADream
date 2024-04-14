// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "interface/WPInteractionInterface.h"
#include "WPChestActor.generated.h"

USTRUCT(BlueprintType)
struct FChestDropItem
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FName ItemName;

	UPROPERTY(EditAnywhere)
	uint32 Amount = 1;

};

UCLASS()
class WINTERPROJECT_API AWPChestActor : public AActor, public IWPInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWPChestActor();
	virtual void Interact(class AWPPlayerActor* PlayerActor) override;

protected:
	UPROPERTY(EditAnyWhere)
	TObjectPtr<USkeletalMeshComponent> MeshComponent;

	UPROPERTY(EditAnyWhere)
	TObjectPtr<class UNiagaraComponent> EffectComponent;

	UPROPERTY(EditAnyWhere)
	TArray<FChestDropItem> ChestDropItems;
	UPROPERTY(EditAnyWhere)
	uint32 Amount;
	UPROPERTY(VisibleAnyWhere)
	uint8 IsOpened : 1;

	UPROPERTY(EditAnyWhere)
	int64 MinGold;

	UPROPERTY(EditAnyWhere)
	int64 MaxGold;

};
