// Fill out your copyright notice in the Description page of Project Settings.


#include "generateMap/WPPathWallBreaker.h"
#include "Components/BoxComponent.h"
#include <struct/WPWallActor.h>

// Sets default values
AWPPathWallBreaker::AWPPathWallBreaker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BreakTriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BreakTriggerComponent"));
	RootComponent = BreakTriggerComponent;
	BreakTriggerComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BreakTriggerComponent->SetGenerateOverlapEvents(true);

	isOverlapStart = 0;

}

// Called when the game starts or when spawned
void AWPPathWallBreaker::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWPPathWallBreaker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isOverlapStart < 1) {
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

