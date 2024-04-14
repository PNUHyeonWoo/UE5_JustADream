// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/Boss/WPBossTrigger.h"
#include "Components/BoxComponent.h"
#include "player/WPPlayerActor.h"
#include "interface/WPBossInterface.h"
#include "interaction/WPIronBarDoorActor.h"

// Sets default values
AWPBossTrigger::AWPBossTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	TriggerComp = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComp"));
	RootComponent = TriggerComp;
	TriggerComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	TriggerComp->SetGenerateOverlapEvents(true);

}

void AWPBossTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (Cast<AWPPlayerActor>(OtherActor))
	{
		if (Cast<IWPBossInterface>(BossActor))
			Cast<IWPBossInterface>(BossActor)->ActivateBoss();

		TriggerComp->SetGenerateOverlapEvents(false);

		if(IronBarDoor)
			IronBarDoor->SetDoorOpen(false);

		Destroy();
	}
}

// Called when the game starts or when spawned
void AWPBossTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame

