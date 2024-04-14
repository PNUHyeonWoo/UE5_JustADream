// Fill out your copyright notice in the Description page of Project Settings.


#include "test/WPTestBoxSheildComponent.h"

void UWPTestBoxSheildComponent::BeginPlay()
{
	Super::BeginPlay();

	CanDeadShield = 0;

}

void UWPTestBoxSheildComponent::WPTakeDamage(float Damage, float Penetration, uint8 HitsWeakness, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount, FVector HitDirection, FVector HitPosition)
{
	IWPAttackTargetInterface::WPTakeDamage(Damage, Penetration, HitsWeakness, StatusEffectNames, StatusEffectAmount, HitDirection, HitPosition);
	UE_LOG(LogTemp, Log, TEXT("%s comp: HP=%f IsDead=%d"), *(GetFName().ToString()), CurrentHP, IsDead);
}

void UWPTestBoxSheildComponent::Die()
{
	UE_LOG(LogTemp, Log, TEXT("%s comp: Call Die"), *(GetFName().ToString()));
}
