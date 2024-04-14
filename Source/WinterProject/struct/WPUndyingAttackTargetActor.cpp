// Fill out your copyright notice in the Description page of Project Settings.


#include "struct/WPUndyingAttackTargetActor.h"
#include <NiagaraFunctionLibrary.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AWPUndyingAttackTargetActor::AWPUndyingAttackTargetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefenceProperty = 10;

}

void AWPUndyingAttackTargetActor::WPTakeDamage(float Damage, float Penetration, uint8 HitsWeakness, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount, FVector HitDirection, FVector HitPosition)
{
	if (Damage > 10 && HitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, HitPosition,(-HitDirection).Rotation());
	}

	if (Damage > 10 && HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, HitPosition);
	}
}

void AWPUndyingAttackTargetActor::Die()
{
}

// Called when the game starts or when spawned
void AWPUndyingAttackTargetActor::BeginPlay()
{
	Super::BeginPlay();
	Defence = DefenceProperty;
}


