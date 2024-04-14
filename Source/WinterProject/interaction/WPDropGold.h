// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WPDropGold.generated.h"

UCLASS()
class WINTERPROJECT_API AWPDropGold : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWPDropGold();

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	static AWPDropGold* CreateDropGold(UObject* WorldObject,uint32 GoldAmount, FVector Location);


protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USphereComponent> TriggerComponent;

public:
	UPROPERTY(EditAnywhere)
	uint32 Gold;

};
