// Fill out your copyright notice in the Description page of Project Settings.


#include "interaction/WPIronBarDoorActor.h"

// Sets default values
AWPIronBarDoorActor::AWPIronBarDoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWPIronBarDoorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWPIronBarDoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWPIronBarDoorActor::SetDoorOpen_Implementation(bool Value)
{
}

