// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "struct/WPUndyingAttackTargetActor.h"
#include "WPWallActor.generated.h"

UCLASS()
class WINTERPROJECT_API AWPWallActor : public AWPUndyingAttackTargetActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWPWallActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	uint8 bIsPassWall;

protected:
	UPROPERTY(EditAnyWhere)
	TObjectPtr<class UStaticMeshComponent> WallMeshComponent;

	UPROPERTY(EditAnyWhere)
	TObjectPtr<class UStaticMeshComponent> SightMeshComponent;


};
