// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/Boss/WPSwordMasterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

void UWPSwordMasterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	APawn* Pawn = TryGetPawnOwner();

	if (Pawn)
	{
		FVector2D Velocity2D(Pawn->GetVelocity());

		Speed = Velocity2D.Length() / Pawn->GetMovementComponent()->GetMaxSpeed();

		FVector2D ControlRotation2D(FRotationMatrix(Pawn->GetActorRotation()).GetUnitAxis(EAxis::X));

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
			MoveAngle = Angle;
		}

		APlayerController* PC = GetWorld()->GetFirstPlayerController();

		if (PC)
		{
			APawn* PPawn = PC->GetPawn();

			if (PPawn)
			{
				float P2P = (PPawn->GetActorLocation() - Pawn->GetActorLocation()).Length();

				const float AttackDis = 100;

				CanAttack = P2P < AttackDis;
			}
		}


	}
}
