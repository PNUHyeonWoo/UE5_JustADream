// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "interface/WPInteractionInterface.h"
#include "WPDropGunActor.generated.h"

UCLASS()
class WINTERPROJECT_API AWPDropGunActor : public AActor, public IWPInteractionInterface
{
	GENERATED_BODY()

public:
	AWPDropGunActor();
	
public:
	virtual void Interact(class AWPPlayerActor* PlayerActor) override;

protected:
	UPROPERTY(EditAnyWhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnyWhere)
	FName GunName;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundWave> GetSound;

};
