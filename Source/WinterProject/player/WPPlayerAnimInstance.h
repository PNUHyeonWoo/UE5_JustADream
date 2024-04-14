// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WPPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeUpdateAnimation(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void SetAllReloadBoolFalse();
	UFUNCTION(BlueprintCallable)
	void ChamberLoad();
	UFUNCTION(BlueprintCallable)
	void SwapGun();
	UFUNCTION(BlueprintCallable)
	void Reload(uint8 ReloadType);
	UFUNCTION(BlueprintCallable)
	void SetGunAnimInstanceBolt(bool Value);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 AnimationIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BasedOnRunningSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RelativeMovingAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	uint8 TryToNormalReload : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	uint8 TryToFastReload : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	uint8 TryToChamberReload : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 PressBoltKey : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 PressSwapKey : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 IsDead : 1;
	
};
