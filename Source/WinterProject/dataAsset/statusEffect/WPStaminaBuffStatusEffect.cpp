// Fill out your copyright notice in the Description page of Project Settings.


#include "dataAsset/statusEffect/WPStaminaBuffStatusEffect.h"
#include "player/WPPlayerActor.h"

float UWPStaminaBuffStatusEffect::TickStatusEffect(AWPPlayerActor* Player, float Amount, float Resistance, float DeltaTime)
{
	if (Amount > 0)
		Player->SetStaminaConsumption(0);
	else
		Player->SetStaminaConsumption(10);

	return Amount-DeltaTime;
}
