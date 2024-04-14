// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WPDestroyTutorialExplanation.generated.h"

UCLASS()
class WINTERPROJECT_API AWPDestroyTutorialExplanation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWPDestroyTutorialExplanation();

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
