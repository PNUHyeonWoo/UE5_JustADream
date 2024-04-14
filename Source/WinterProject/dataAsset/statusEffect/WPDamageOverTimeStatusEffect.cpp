// Fill out your copyright notice in the Description page of Project Settings.


#include "dataAsset/statusEffect/WPDamageOverTimeStatusEffect.h"
#include "player/WPPlayerActor.h"

float UWPDamageOverTimeStatusEffect::TickStatusEffect(AWPPlayerActor* Player, float Amount, float Resistance, float DeltaTime)
{
	if (Amount > 0)
		Player->WPTakeDamage((BasicDamage + Amount * DamageWeight) * DeltaTime, 0, 1, {}, {}, FVector(0,0,0), FVector(0,0,0));

	float NewAmount = Amount - DeltaTime * AmountReduction;

	return NewAmount < 0 ? 0 : NewAmount;
}
