// Fill out your copyright notice in the Description page of Project Settings.


#include "generateMap/WPPath.h"
#include "Components/BoxComponent.h"
#include "struct/WPWallActor.h"
#include "player/WPPlayerActor.h"

// Sets default values
AWPPath::AWPPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	PathMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PathMesh"));
	PathMesh->SetupAttachment(RootComponent);

	CollisionComponent->SetGenerateOverlapEvents(true);

	ClosetPositionComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ClosetPositionComponent"));
	ClosetPositionComponent->SetupAttachment(RootComponent);

	Wall1PositionComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Wall1PositionComponent"));
	Wall1PositionComponent->SetupAttachment(RootComponent);

	Wall2PositionComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Wall2PositionComponent"));
	Wall2PositionComponent->SetupAttachment(RootComponent);

	Door1PositionComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Door1PositionComponent"));
	Door1PositionComponent->SetupAttachment(RootComponent);

	Door2PositionComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Door2PositionComponent"));
	Door2PositionComponent->SetupAttachment(RootComponent);



	isOverlapStart = 0;

}

// Called when the game starts or when spawned
void AWPPath::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->SpawnActor<AActor>(ClosetActorClass, ClosetPositionComponent->GetComponentLocation(), ClosetPositionComponent->GetComponentRotation());
	GetWorld()->SpawnActor<AActor>(WallActorClass, Wall1PositionComponent->GetComponentLocation(), Wall1PositionComponent->GetComponentRotation());
	GetWorld()->SpawnActor<AActor>(WallActorClass, Wall2PositionComponent->GetComponentLocation(), Wall2PositionComponent->GetComponentRotation());
	GetWorld()->SpawnActor<AActor>(DoorActorClass, Door1PositionComponent->GetComponentLocation(), Door1PositionComponent->GetComponentRotation());
	GetWorld()->SpawnActor<AActor>(DoorActorClass, Door2PositionComponent->GetComponentLocation(), Door2PositionComponent->GetComponentRotation());
}

void AWPPath::LiftPathFog()
{
	Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn())->LiftPathFog(PathIndex);
}

// Called every frame
void AWPPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isOverlapStart < 2) {
		// 이 액터와 겹치는 PathWall 액터를 파괴한다.
		TArray<AActor*> OverlappingActors;
		GetOverlappingActors(OverlappingActors, AWPWallActor::StaticClass());
		for (AActor* OverlappingActor : OverlappingActors)
		{
			if (Cast<AWPWallActor>(OverlappingActor)->bIsPassWall)
			{
				isOverlapStart++;
				OverlappingActor->Destroy();
			}
		}
	}
}