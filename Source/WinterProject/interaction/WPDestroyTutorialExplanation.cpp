// Fill out your copyright notice in the Description page of Project Settings.


#include "interaction/WPDestroyTutorialExplanation.h"
#include "player/WPPlayerActor.h"

// Sets default values
AWPDestroyTutorialExplanation::AWPDestroyTutorialExplanation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AWPDestroyTutorialExplanation::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AWPPlayerActor* PlayerActor = Cast<AWPPlayerActor>(OtherActor);

	if (PlayerActor)
		PlayerActor->SetTutorialWidget(TEXT(""));
}

// Called when the game starts or when spawned
void AWPDestroyTutorialExplanation::BeginPlay()
{
	Super::BeginPlay();
	
}



