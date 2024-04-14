// Fill out your copyright notice in the Description page of Project Settings.


#include "interaction/WPCreateTutorialExplanation.h"
#include "player/WPPlayerActor.h"

// Sets default values
AWPCreateTutorialExplanation::AWPCreateTutorialExplanation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AWPCreateTutorialExplanation::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AWPPlayerActor* PlayerActor = Cast<AWPPlayerActor>(OtherActor);

	if (PlayerActor)
		PlayerActor->SetTutorialWidget(StringTableId);
}

// Called when the game starts or when spawned
void AWPCreateTutorialExplanation::BeginPlay()
{
	Super::BeginPlay();
	
}


