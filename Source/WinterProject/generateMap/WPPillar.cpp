// Fill out your copyright notice in the Description page of Project Settings.


#include "generateMap/WPPillar.h"
#include "Components/BoxComponent.h"

#include "Engine/Level.h"

// Sets default values
AWPPillar::AWPPillar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillarMesh"));
	StaticMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AWPPillar::BeginPlay()
{
	Super::BeginPlay();

	// drawdebugcircle
	DrawDebugCircle(GetWorld(), GetActorLocation(), Radius, 50, FColor::Red, true, -1.f, 0, 5.f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f), false);
	
}

// Called every frame
void AWPPillar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

