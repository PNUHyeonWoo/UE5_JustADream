// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WPAttackTargetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWPAttackTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WINTERPROJECT_API IWPAttackTargetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	float GetMaxHP();
	void SetMaxHP(float NewMaxHP);
	float GetCurrentHP();
	float SetCurrentHP(float NewHP);
	float AddToCurrentHP(float Value);
	float GetDefence();
	void SetDefence(float Value);
	uint8 GetCanDeadShield(); 
	uint8 GetIsDead();
	uint8 GetHasWeakness();
	UPrimitiveComponent* GetWeaknessComponent();
	void SetWeaknessComponent(UPrimitiveComponent* NewWeakenessComponent);

public:
	virtual void WPTakeDamage(float Damage, float Penetration,uint8 HitsWeakness, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount, FVector HitDirection, FVector HitPosition);
	virtual void Die() = 0;

public:
	static uint32 RaycastAttack(AActor* Attacker, FVector Start, FVector End, float Damage, float Penetration, TArray<IWPAttackTargetInterface*> Targets = {}, TArray<IWPAttackTargetInterface*> Exclusions = {}, TArray<FName> StatusEffectNames = {}, TArray<float> StatusEffectAmount = {},FVector* LastHitPositon = nullptr);
	static uint32 ShapecastAttack(AActor* Attacker, const FCollisionShape& CollisionShape, FVector Position, float Damage, float Penetration, TArray<IWPAttackTargetInterface*> Targets = {}, TArray<IWPAttackTargetInterface*> Exclusions = {}, TArray<FName> StatusEffectNames = {}, TArray<float> StatusEffectAmount = {});
	static uint32 SpherecastAttack(AActor* Attacker, FVector Position,float Radius, float Damage, float Penetration, TArray<IWPAttackTargetInterface*> Targets = {}, TArray<IWPAttackTargetInterface*> Exclusions = {}, TArray<FName> StatusEffectNames = {}, TArray<float> StatusEffectAmount = {});

private:
	static uint8 IsValidTarget(IWPAttackTargetInterface* TargetInterface, TArray<IWPAttackTargetInterface*> Targets, TArray<IWPAttackTargetInterface*> Exclusions);
protected:
	float MaxHP = 100;
	float CurrentHP = 100;
	float Defence = 0;
	uint8 CanDeadShield = 1;
	uint8 IsDead = 0;
	uint8 HasWeakness = 0;
	UPrimitiveComponent* WeaknessComponent = nullptr;
};
