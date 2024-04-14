// Fill out your copyright notice in the Description page of Project Settings.


#include "interaction/WPDropGold.h"
#include "Components/SphereComponent.h"
#include "player/WPPlayerActor.h"

// Sets default values
AWPDropGold::AWPDropGold()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	MeshComponent->SetEnableGravity(true);
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->bUseDefaultCollision = true;
	MeshComponent->SetCanEverAffectNavigation(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> GoldMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/struct/Mesh/Gold.Gold'"));
	if (GoldMeshRef.Object)
	{
		MeshComponent->SetStaticMesh(GoldMeshRef.Object);
	}

	TriggerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerComponent"));
	TriggerComponent->SetupAttachment(RootComponent);
	TriggerComponent->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	TriggerComponent->SetGenerateOverlapEvents(true);
	TriggerComponent->SetSphereRadius(96);
	TriggerComponent->SetCanEverAffectNavigation(false);

	Tags.Add(TEXT("ky_shot"));
	
}

void AWPDropGold::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	AWPPlayerActor* PlayerActor = Cast<AWPPlayerActor>(OtherActor);
	if (PlayerActor)
	{
		PlayerActor->ReceiveGold(Gold);
		Destroy();
	}
}

AWPDropGold* AWPDropGold::CreateDropGold(UObject* WorldObject, uint32 GoldAmount, FVector Location)
{
	AWPDropGold* NewDropGold = WorldObject->GetWorld()->SpawnActor<AWPDropGold>(AWPDropGold::StaticClass(), Location, FRotator());
	if(NewDropGold)
		NewDropGold->Gold = GoldAmount;
	else 
	{
		NewDropGold = WorldObject->GetWorld()->SpawnActor<AWPDropGold>(AWPDropGold::StaticClass(), Location + FVector(0,0,150), FRotator());
		if(NewDropGold)
			NewDropGold->Gold = GoldAmount;
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to Create Gold."));
			return nullptr;
		}
	}
	return NewDropGold;
}


