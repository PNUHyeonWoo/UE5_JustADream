// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WPPlayerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	static UWPPlayerSaveGame* CreateNewPlayerSaveGame();
	void FillAmmoInGun(uint8 GunType);
	void FillAmmoInGear(uint8 GunType);
	class UWPGunDataAsset* GetEquippedGunData();

public:
	UPROPERTY(VisibleAnyWhere)
	uint8 CurrentFloor;

	UPROPERTY(VisibleAnyWhere)
	uint32 Gold;

	UPROPERTY(VisibleAnyWhere)
	TMap<FName, uint8> GunPurchase;

	UPROPERTY(VisibleAnyWhere)
	uint8 GearLevel;

	UPROPERTY(VisibleAnyWhere)
	TArray<FName> PossessedGuns;

	UPROPERTY(VisibleAnyWhere)
	uint8 EquippedGun;

	UPROPERTY(VisibleAnyWhere)
	TArray<uint8> NumOfAmmoInChamber;

	UPROPERTY(VisibleAnyWhere)
	TArray<uint8> NumOfAmmoInMagazine;

	UPROPERTY(VisibleAnyWhere)
	TArray<int8> AmmoInGearUpper;

	UPROPERTY(VisibleAnyWhere)
	TArray<int8> AmmoInGearLower;

	UPROPERTY(VisibleAnyWhere)
	TArray<TObjectPtr<class UWPItemDataAsset>> ItemsInGear;

	UPROPERTY(VisibleAnyWhere)
	TArray<float> ItemAmounts;

	UPROPERTY(VisibleAnyWhere)
	float MaxHP;

	UPROPERTY(VisibleAnyWhere)
	float CurrentHP;

	UPROPERTY(VisibleAnyWhere)
	float MaxStamina;

	UPROPERTY(VisibleAnyWhere)
	float CurrentStamina;

	UPROPERTY(VisibleAnyWhere)
	TMap<FName, float> StatusEffect;

	UPROPERTY(VisibleAnyWhere)
	TMap<FName,float> StatusEffectResistance;

	UPROPERTY(VisibleAnyWhere)
	float StaminaRecovery;

	UPROPERTY(VisibleAnyWhere)
	float StaminaConsumption;

	UPROPERTY(VisibleAnyWhere)
	float WalkMaxSpeed;

	UPROPERTY(VisibleAnyWhere)
	float DefaultWalkMaxSpeed;

	UPROPERTY(VisibleAnyWhere)
	float RunAcceleration;

	UPROPERTY(VisibleAnyWhere)
	float DefaultRunAcceleration;

	UPROPERTY(VisibleAnyWhere)
	float RunMaxSpeed;

	UPROPERTY(VisibleAnyWhere)
	float DefaultRunMaxSpeed;

	UPROPERTY(VisibleAnyWhere)
	float AimWalkMaxSpeed;

	UPROPERTY(VisibleAnyWhere)
	float DefaultAimWalkMaxSpeed;

	UPROPERTY(VisibleAnyWhere)
	float RecoilControl;

	UPROPERTY(VisibleAnyWhere)
	float ReloadSpeed;

	UPROPERTY(VisibleAnyWhere)
	float DiscountRate;

	UPROPERTY(VisibleAnyWhere)
	float DropRate;

	UPROPERTY(VisibleAnyWhere)
	float CumulativeEXP;



	
};
