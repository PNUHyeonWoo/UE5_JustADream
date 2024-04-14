// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "interface/WPAttackTargetInterface.h"
#include "WPPlayerActor.generated.h"

UCLASS()
class WINTERPROJECT_API AWPPlayerActor : public ACharacter, public IWPAttackTargetInterface
{
	GENERATED_BODY()

public:

	AWPPlayerActor();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void WPTakeDamage(float Damage, float Penetration, uint8 HitsWeakness, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount, FVector HitDirection, FVector HitPosition) override;
	virtual void Die();

public:
	float GetBasedOnRunningSpeed();
	void SetPlayerHUIUserWidget(class UWPPlayerHUIUserWidget* newWidget);
	uint8 GetIsTrueAiming();
	void FillAmmo(uint8 GunType);
	void SwapGun();
	void ChamberLoad();
	void ClickItem(uint8 isLeftClick,uint8 YIndex,uint8 XIndex);
	const class UWPPlayerSaveGame* GetPlayerSaveGame();
	class UWPGunDataAsset* GetEquippedGunData();
	void Reload(uint8 ReloadType);
	void SetGunAnimInstanceBolt(bool Value);
	bool GetGunAnimInstanceBolt();
	void SavePlayerStat();
	bool IsRealesed();
	void SetPlayerInputToFalsePerpect();
	void SetPlayerInputToTruePerpect();
	void SetPlayerInput(bool Value);
	void OpenShop();
	bool TryToSpendGold(uint32 Value);
	void ReceiveGold(uint32 Value);
	void SetGunAsPurchase(FName GunName);
	bool TryToReceiveGun(FName GunName);
	bool TryToReceiveItem(FName ItemName,float Amount = -1);
	void UpgradeGear();
	int8 GetEmptyItemSpace();
	void SetStatusEffect(FName StatusEffectName, float Value);
	void SetStaminaConsumption(float Consumption);
	void SetMoveSpeed(float Ratio);
	float GetMaxRunSpeed();
	void GenerateRoomForMap(TArray<class UWPArena*> Arenas);
	void GeneratePathForMap(TArray<FVector2D> Paths);
	float GetCumulativeEXP();
	void SetLoadingScreenToFalse();
	class UNiagaraSystem* GetDefaultHitEffect();
	void SetRemainMagazineAmmoVisible(uint8 Y, uint8 X, bool Value);
	void LiftPathFog(FVector2D PathIndex);
	void SetBossIcon(uint8 Index);
	void SetTutorialWidget(FString Id);

protected:
	void ReceiveGun(FName GunName);
	void initGun();
	void SetGunPosition();
	void RotatePlayer(FName NowState);
	void AimWithShoulder();

	void MovePlayer(FName NowState);
	void TickStamina(float DeltaTime);
	void TickStatusEffect(float DeltaTime);

	void SetIsTrueAiming(uint8 value);

	FVector GetMouseAimPosition(float Height);

	void SetIsShotCoolTimeFalse();
	void SetCanRecoverStaminaTrue();
	void OpenPlayerDeadWidget();

	//key mapping functions

	void InputMoveY(float Value);
	void InputMoveX(float Value);
	void PressRun();
	void ReleaseRun();
	void PressBolt();
	void ReleaseBolt();
	void PressSwap();
	void ReleaseSwap();
	void PressShot();
	void PressAim();
	void PressInteraction();
	void PressMap();
	void PressMenu();



protected:
	//component Section----------

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USkeletalMeshComponent> MainGunMeshComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USkeletalMeshComponent> SubGunMeshComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCapsuleComponent> WeaknessCapsuleComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCameraComponent> CameraComponent;

	//Move Section-----------

	UPROPERTY()
	uint8 IsRunning : 1;

	UPROPERTY()
	uint8 IsTrueAiming : 1;

	UPROPERTY()
	FVector2D MoveDirection;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> HitEffect;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> DefaultAttackHitEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundWave> HitSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundWave> DieSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundWave> EmptyAmmoSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundWave> GoldSound;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> RayEffectActor;

	UPROPERTY(EditAnywhere)
	float RayEffectWidth = 0.015;

	UPROPERTY(EditAnywhere)
	float RayEffectBuckShotWidth = 0.005;

	UPROPERTY(EditAnywhere)
	TArray<float> ShoulderLengths;

	UPROPERTY(VisibleAnyWhere)
	float ShoulderLength;

	UPROPERTY(EditAnyWhere)
	float ShoulderRelativeHeight;

	UPROPERTY(VisibleAnyWhere)
	float ShoulderHeight = 210;

	UPROPERTY()
	FTimerHandle StaminaRecoveryHandle;

	UPROPERTY(VisibleAnyWhere)
	uint8 CanRecoverStamina;

	UPROPERTY(VisibleAnyWhere)
	uint8 CanRun;


	//-----------------

	UPROPERTY(VisibleAnyWhere)
	TObjectPtr<class UWPPlayerSaveGame> PlayerSaveGame;

	

	TTuple<uint8, uint8> AmmoToReload;

	UPROPERTY()
	FTimerHandle ShotCoolTimeHandle;

	UPROPERTY()
	FTimerHandle DeadWidgetHandle;

	UPROPERTY(VisibleAnyWhere)
	uint8 IsShotCoolTime;


	UPROPERTY()
	TObjectPtr<class UWPPlayerHUIUserWidget> PlayerHUIUserWidget;


	bool bInitStatusEffectImages = false;
	

};
