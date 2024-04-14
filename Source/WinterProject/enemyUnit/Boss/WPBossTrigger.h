// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WPBossTrigger.generated.h"

UCLASS()
class WINTERPROJECT_API AWPBossTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWPBossTrigger();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnyWhere)
	TObjectPtr<class UBoxComponent> TriggerComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> BossActor;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class AWPIronBarDoorActor> IronBarDoor;

};
