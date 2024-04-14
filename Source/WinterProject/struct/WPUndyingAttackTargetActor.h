// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "interface/WPAttackTargetInterface.h"
#include "WPUndyingAttackTargetActor.generated.h"

UCLASS()
class WINTERPROJECT_API AWPUndyingAttackTargetActor : public AActor, public IWPAttackTargetInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWPUndyingAttackTargetActor();

public:
	virtual void WPTakeDamage(float Damage, float Penetration, uint8 HitsWeakness, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount, FVector HitDirection, FVector HitPosition) override;
	virtual void Die() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnyWhere)
	float DefenceProperty;
	UPROPERTY(EditAnyWhere)
	TObjectPtr<class UNiagaraSystem> HitEffect;

	UPROPERTY(EditAnyWhere)
	TObjectPtr<class USoundBase> HitSound;

};
