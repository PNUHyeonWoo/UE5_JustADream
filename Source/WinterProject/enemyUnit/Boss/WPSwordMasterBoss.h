// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "interface/WPAttackTargetInterface.h"
#include "interface/WPBossInterface.h"
#include "WPSwordMasterBoss.generated.h"

UCLASS()
class WINTERPROJECT_API AWPSwordMasterBoss : public ACharacter, public IWPAttackTargetInterface, public IWPBossInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWPSwordMasterBoss();

public:
	virtual void WPTakeDamage(float Damage, float Penetration, uint8 HitsWeakness, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount, FVector HitDirection, FVector HitPosition) override;
	virtual void Die() override;

	virtual void ActivateBoss() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAudioComponent> AudioComp;

	UPROPERTY(VisibleAnyWhere)
	uint8 bActivate : 1;

	UPROPERTY()
	FVector CumulativeKnockback;

	UPROPERTY(EditAnywhere)
	TArray<class USoundWave*> HitSounds;

	UPROPERTY()
	uint8 SoundIndex;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> HitEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class AWPIronBarDoorActor> IronBarDoor;
		

};
