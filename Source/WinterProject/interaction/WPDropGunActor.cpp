// Fill out your copyright notice in the Description page of Project Settings.


#include "interaction/WPDropGunActor.h"
#include "player/WPPlayerActor.h"
#include <Kismet/GameplayStatics.h>



AWPDropGunActor::AWPDropGunActor()
{
	PrimaryActorTick.bCanEverTick = false;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

void AWPDropGunActor::Interact(AWPPlayerActor* PlayerActor)
{
	if(PlayerActor->TryToReceiveGun(GunName))
		if(GetSound)
			UGameplayStatics::PlaySoundAtLocation(this, GetSound, GetActorLocation());
}
