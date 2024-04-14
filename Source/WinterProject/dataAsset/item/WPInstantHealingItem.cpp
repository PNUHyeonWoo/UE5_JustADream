// Fill out your copyright notice in the Description page of Project Settings.


#include "dataAsset/item/WPInstantHealingItem.h"
#include "interface/WPAttackTargetInterface.h"

float UWPInstantHealingItem::UseItem(AActor* PlayerActor, float RemainAmount)
{
	IWPAttackTargetInterface* AttackTarget = Cast<IWPAttackTargetInterface>(PlayerActor);

	if (AttackTarget->GetCurrentHP() < AttackTarget->GetMaxHP())
	{
		AttackTarget->AddToCurrentHP(HealAmount);
		return RemainAmount - 1;
	}

	return RemainAmount;
}
