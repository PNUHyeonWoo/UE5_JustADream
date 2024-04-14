// Fill out your copyright notice in the Description page of Project Settings.


#include "interaction/WPShopActor.h"
#include "player//WPPlayerActor.h"
#include <Kismet/GameplayStatics.h>

void AWPShopActor::Interact(AWPPlayerActor* PlayerActor)
{
	PlayerActor->OpenShop();

	if(PlayerActor->IsRealesed() && OpenSound)
		UGameplayStatics::PlaySoundAtLocation(this, OpenSound, GetActorLocation());
}
