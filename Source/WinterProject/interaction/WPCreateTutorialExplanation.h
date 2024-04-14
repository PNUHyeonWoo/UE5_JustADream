// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WPCreateTutorialExplanation.generated.h"

UCLASS()
class WINTERPROJECT_API AWPCreateTutorialExplanation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWPCreateTutorialExplanation();

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnyWhere)
	FString StringTableId;

};
