// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WPGunDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPGunDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	static UWPGunDataAsset* GetGunData(FName GunName);


public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> GunMesh;

	UPROPERTY(EditAnywhere)
	int8 AnimationIndex; //main gun : 0~49 , sub gun : 50 ~ 127

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> MagazineMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UWPGunAnimInstance> GunAnimationBP;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> GunImage;

	UPROPERTY(EditAnywhere)
	uint8 GunType; // 0: main , 1: sub

	UPROPERTY(EditAnywhere)
	uint8 DoAutoChamberLoad;

	UPROPERTY(EditAnywhere)
	uint8 ChamberCapacity;

	UPROPERTY(EditAnywhere)
	uint8 MagazineCapacity;

	UPROPERTY(EditAnywhere)
	uint8 CanNormalReload;

	UPROPERTY(EditAnywhere)
	uint8 CanFastReload;

	UPROPERTY(EditAnywhere)
	uint8 CanChamberReload;

	UPROPERTY(EditAnywhere)
	uint8 CanReloadMagazineThroughChamber;

	UPROPERTY(EditAnywhere)
	uint8 CanRackBolt;

	UPROPERTY(EditAnywhere)
	float ShotCoolTime;

	UPROPERTY(EditAnywhere)
	float Recoil;

	UPROPERTY(EditAnyWhere)
	float Spread;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundWave> FireSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundWave> BoltOnSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundWave> BoltOffSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> MuzzleFlashEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UWPAmmoDataAsset> AmmoUsed;

	UPROPERTY(EditAnyWhere)
	TArray<uint8> NumOfAmmoInGear;

	UPROPERTY(EditAnywhere)
	uint32 Price;

	UPROPERTY(EditAnywhere)
	uint32 AmmoPrice;
	
};
