// Fill out your copyright notice in the Description page of Project Settings.


#include "dataAsset/statusEffect/WPSpeedBuffStatusEffect.h"
#include "player/WPPlayerActor.h"

float UWPSpeedBuffStatusEffect::TickStatusEffect(AWPPlayerActor* Player, float Amount, float Resistance, float DeltaTime)
{
	if (Amount > 0)
		Player->SetMoveSpeed(SpeedRatio);
	else
		Player->SetMoveSpeed(1);

	return Amount - DeltaTime;
}
