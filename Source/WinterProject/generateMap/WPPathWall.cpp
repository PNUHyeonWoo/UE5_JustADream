// Fill out your copyright notice in the Description page of Project Settings.


#include "generateMap/WPPathWall.h"
#include "Components/BoxComponent.h"

// Sets default values
AWPPathWall::AWPPathWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	CollisionComponent->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void AWPPathWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWPPathWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

