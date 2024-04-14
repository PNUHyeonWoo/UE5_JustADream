// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "interface/WPAttackTargetInterface.h"
#include "WPShieldComponent.generated.h"

class USoundBase;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WINTERPROJECT_API UWPShieldComponent : public UStaticMeshComponent, public IWPAttackTargetInterface
{
	GENERATED_BODY()

public:
	UWPShieldComponent();

	virtual void PostLoad() override;

	virtual void BeginPlay() override;

	virtual void Die() override;

	virtual void WPTakeDamage(float Damage, float Penetration, uint8 HitsWeakness, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount, FVector HitDirection, FVector HitPosition) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float I_MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float I_CurrentHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float I_Defence = 2.0f;

	UPROPERTY(EditAnywhere)
	USoundBase* ShieldHitSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ShieldBreakSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> HitEffect;
	
};
