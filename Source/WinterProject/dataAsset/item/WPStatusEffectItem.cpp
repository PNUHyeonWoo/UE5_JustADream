// Fill out your copyright notice in the Description page of Project Settings.


#include "dataAsset/item/WPStatusEffectItem.h"
#include "player/WPPlayerActor.h"

float UWPStatusEffectItem::UseItem(AActor* PlayerActor, float RemainAmount)
{
	Cast<AWPPlayerActor>(PlayerActor)->SetStatusEffect(FName(StatusEffectName), ValueToApply);
	return RemainAmount - 1;
}
