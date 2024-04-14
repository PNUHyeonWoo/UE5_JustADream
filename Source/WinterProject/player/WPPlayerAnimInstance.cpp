// Fill out your copyright notice in the Description page of Project Settings.


#include "player/WPPlayerAnimInstance.h"
#include "player/WPPlayerActor.h"

void UWPPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	APawn* Pawn = TryGetPawnOwner();

	if (Pawn)
	{
		FVector2D Velocity2D(Pawn->GetVelocity());
		FVector2D ControlRotation2D(FRotationMatrix(Pawn->GetActorRotation()).GetUnitAxis(EAxis::X));

		Speed = Velocity2D.Length() / Cast<AWPPlayerActor>(Pawn)->GetMaxRunSpeed();
		BasedOnRunningSpeed = Cast<AWPPlayerActor>(Pawn)->GetBasedOnRunningSpeed();

		Velocity2D.Normalize();
		ControlRotation2D.Normalize();

		if (Speed > 0.01f)
		{
			float Angle = acosf(Velocity2D.Dot(ControlRotation2D)) * 180.0f / PI;
			float Det = ControlRotation2D.X * Velocity2D.Y - ControlRotation2D.Y * Velocity2D.X;
			if (Det > 0)
			{
				Angle = 360 - Angle;
			}
			RelativeMovingAngle = Angle;
		}

	}
}

void UWPPlayerAnimInstance::SetAllReloadBoolFalse()
{
	TryToNormalReload = 0;
	TryToFastReload = 0;
	TryToChamberReload = 0;
}

void UWPPlayerAnimInstance::ChamberLoad()
{
	Cast<AWPPlayerActor>(TryGetPawnOwner())->ChamberLoad();
}

void UWPPlayerAnimInstance::SwapGun()
{
	Cast<AWPPlayerActor>(TryGetPawnOwner())->SwapGun();
}

void UWPPlayerAnimInstance::Reload(uint8 ReloadType)
{
	Cast<AWPPlayerActor>(TryGetPawnOwner())->Reload(ReloadType);
}

void UWPPlayerAnimInstance::SetGunAnimInstanceBolt(bool Value)
{
	Cast<AWPPlayerActor>(TryGetPawnOwner())->SetGunAnimInstanceBolt(Value);
}
