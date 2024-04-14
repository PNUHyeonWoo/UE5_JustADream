// Fill out your copyright notice in the Description page of Project Settings.


#include "struct/WPWallActor.h"

// Sets default values
AWPWallActor::AWPWallActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WallMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallComponent"));
	RootComponent = WallMeshComponent;
	SightMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SightComponent"));
	SightMeshComponent->SetupAttachment(RootComponent);

	SightMeshComponent->SetVisibility(false);
}

// Called when the game starts or when spawned
void AWPWallActor::BeginPlay()
{
	AWPUndyingAttackTargetActor::BeginPlay();
	SightMeshComponent->SetVisibility(true);
}


