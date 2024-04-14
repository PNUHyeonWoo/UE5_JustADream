// Fill out your copyright notice in the Description page of Project Settings.


#include "dataAsset/statusEffect/WPDefenceBuffStatusEffect.h"
#include "player/WPPlayerActor.h"

float UWPDefenceBuffStatusEffect::TickStatusEffect(AWPPlayerActor* Player, float Amount, float Resistance, float DeltaTime)
{
	if (Amount > 0)
		Player->SetDefence(Defence);
	else
		Player->SetDefence(0);

	return Amount - DeltaTime;
}
