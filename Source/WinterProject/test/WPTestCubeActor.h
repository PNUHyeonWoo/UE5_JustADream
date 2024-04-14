// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "interface/WPAttackTargetInterface.h"
#include "WPTestCubeActor.generated.h"

UCLASS()
class WINTERPROJECT_API AWPTestCubeActor : public AActor,public IWPAttackTargetInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWPTestCubeActor();

public:
	virtual void WPTakeDamage(float Damage, float Penetration, uint8 HitsWeakness, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount, FVector HitDirection, FVector HitPosition) override;
	virtual void Die() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void Attack();

public:
	UPROPERTY(EditAnyWhere)
	TObjectPtr<class UBoxComponent> BoxComp;

	UPROPERTY(EditAnyWhere)
	TObjectPtr<class UBoxComponent> WeaknessBoxComp;

	UPROPERTY(EditAnyWhere)
	TObjectPtr<class UWPTestBoxSheildComponent> BoxShieldComp;

	UPROPERTY(EditAnyWhere)
	TArray< TObjectPtr<AActor>> Targets;

	UPROPERTY(EditAnyWhere)
	TArray< TObjectPtr<AActor>> Ex;

	TArray<IWPAttackTargetInterface*> TargetsI;
	TArray<IWPAttackTargetInterface*> ExI;

	UPROPERTY(EditAnyWhere)
	uint8 isAttacker;
	UPROPERTY()
	FTimerHandle AttackTimer;



};
