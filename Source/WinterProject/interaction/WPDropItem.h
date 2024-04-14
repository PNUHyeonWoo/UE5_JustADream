// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "interface/WPInteractionInterface.h"
#include "WPDropItem.generated.h"

UCLASS()
class WINTERPROJECT_API AWPDropItem : public AActor, public IWPInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWPDropItem();
	virtual void Interact(class AWPPlayerActor* PlayerActor) override;

public:
	virtual void Tick(float DeltaTime) override;

	static AWPDropItem* CreateDropItem(UObject* WorldObject, FVector Location, FName ItemName, float Amount = -1 );

protected:
	UPROPERTY(EditAnyWhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	UPROPERTY(EditAnyWhere)
	FName ItemName;
	UPROPERTY(EditAnyWhere)
	float Amount;


};
