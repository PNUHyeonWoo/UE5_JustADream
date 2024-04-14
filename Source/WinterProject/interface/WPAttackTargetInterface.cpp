// Fill out your copyright notice in the Description page of Project Settings.


#include "interface/WPAttackTargetInterface.h"
#include <player/WPPlayerActor.h>
#include "NiagaraFunctionLibrary.h"

// Add default functionality here for any IWPAttackTargetInterface functions that are not pure virtual.

float IWPAttackTargetInterface::GetMaxHP()
{
	return MaxHP;
}

void IWPAttackTargetInterface::SetMaxHP(float NewMaxHP)
{
	MaxHP = NewMaxHP;
	if (MaxHP < CurrentHP)
		CurrentHP = MaxHP;
}

float IWPAttackTargetInterface::GetCurrentHP()
{
	return CurrentHP;
}

float IWPAttackTargetInterface::SetCurrentHP(float NewHP)
{
	CurrentHP = FMath::Clamp(NewHP, 0, MaxHP);
	return CurrentHP;
}

float IWPAttackTargetInterface::AddToCurrentHP(float Value)
{
	CurrentHP = FMath::Clamp(CurrentHP+Value, 0, MaxHP);
	return CurrentHP;
}

float IWPAttackTargetInterface::GetDefence()
{
	return Defence;
}

void IWPAttackTargetInterface::SetDefence(float Value)
{
	Defence = Value;
}

uint8 IWPAttackTargetInterface::GetCanDeadShield()
{
	return CanDeadShield;
}

uint8 IWPAttackTargetInterface::GetIsDead()
{
	return IsDead;
}

uint8 IWPAttackTargetInterface::GetHasWeakness()
{
	return HasWeakness;
}

UPrimitiveComponent* IWPAttackTargetInterface::GetWeaknessComponent()
{
	return WeaknessComponent;
}

void IWPAttackTargetInterface::SetWeaknessComponent(UPrimitiveComponent* NewWeakenessComponent)
{
	if (NewWeakenessComponent)
		HasWeakness = 1;
	else
		HasWeakness = 0;

	WeaknessComponent = NewWeakenessComponent;

}

void IWPAttackTargetInterface::WPTakeDamage(float Damage, float Penetration, uint8 HitsWeakness, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount, FVector HitDirection, FVector HitPosition)
{
	if (IsDead)
		return;

	if (Penetration < Defence)	
		Damage /= FMath::Pow(2, Defence - Penetration);

	if (HasWeakness && !HitsWeakness)
		Damage /= 2;

	AddToCurrentHP(-Damage);

	if (CurrentHP <= 0)
	{
		IsDead = 1;
		Die();
	}

}

uint32 IWPAttackTargetInterface::RaycastAttack(AActor* Attacker, FVector Start, FVector End, float Damage, float Penetration, TArray<IWPAttackTargetInterface*> Targets, TArray<IWPAttackTargetInterface*> Exclusions, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount, FVector* LastHitPosition)
{
	uint32 NumOfHit = 0;

	TArray<FHitResult> HitResults;

	Attacker->GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECC_GameTraceChannel2);

	FVector AttackDirection = End - Start;
	AttackDirection.Normalize();

	bool IsBlock = false;

	for (FHitResult HitResult : HitResults)
	{
		IWPAttackTargetInterface* TargetInterface = Cast<IWPAttackTargetInterface>(HitResult.GetComponent());


		if (!TargetInterface)
		{
			TargetInterface = Cast<IWPAttackTargetInterface>(HitResult.GetActor());
			if (!TargetInterface)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(Attacker->GetWorld(), Cast<AWPPlayerActor>(Attacker->GetWorld()->GetFirstPlayerController()->GetPawn())->GetDefaultHitEffect(), HitResult.ImpactPoint, (-AttackDirection).Rotation());
				if (LastHitPosition)
				{
					*LastHitPosition = HitResult.ImpactPoint;
					IsBlock = true;
				}
				break;
			}
		}


		uint8 isValidTarget = IsValidTarget(TargetInterface, Targets, Exclusions);

		if (!isValidTarget)
			continue;

		//real hit

		if (TargetInterface->GetIsDead() && !TargetInterface->GetCanDeadShield())
		{
				continue;
		}
		else
		{
			uint8 HitsWeakness = 0;

			if (TargetInterface->GetHasWeakness())
			{
				TArray<FHitResult> WeaknessHitResults;

				Attacker->GetWorld()->LineTraceMultiByChannel(WeaknessHitResults, HitResult.ImpactPoint, End, ECC_GameTraceChannel3);

				for(FHitResult WeaknessHitResult : WeaknessHitResults)
					if (WeaknessHitResult.GetComponent() == TargetInterface->GetWeaknessComponent())
					{
						HitsWeakness = 1;
						break;
					}
			}

			TargetInterface->WPTakeDamage(Damage, Penetration, HitsWeakness,StatusEffectNames,StatusEffectAmount,AttackDirection,HitResult.Location);
		}

		NumOfHit++;

		if (TargetInterface->GetDefence() < Penetration) 
		{
			Damage *= 0.9;
			Penetration -= 1;
			continue;
		}
		else
		{
			if (LastHitPosition)
			{
				*LastHitPosition = HitResult.ImpactPoint;
				IsBlock = true;
			}
			break;
		}
	
	}

	if (LastHitPosition && !IsBlock)
	{
		*LastHitPosition = End;
	}


	return NumOfHit;
}

uint32 IWPAttackTargetInterface::ShapecastAttack(AActor* Attacker, const FCollisionShape& CollisionShape, FVector Position, float Damage, float Penetration, TArray<IWPAttackTargetInterface*> Targets, TArray<IWPAttackTargetInterface*> Exclusions, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount)
{
	uint32 NumOfHit = 0;

	TArray<FHitResult> HitResults;

	Attacker->GetWorld()->SweepMultiByChannel(HitResults, Position, Position, FQuat(), ECC_GameTraceChannel2, CollisionShape);

	for (FHitResult HitResult : HitResults) 
	{
		IWPAttackTargetInterface* TargetInterface = Cast<IWPAttackTargetInterface>(HitResult.GetComponent());


		if (!TargetInterface)
		{
			TargetInterface = Cast<IWPAttackTargetInterface>(HitResult.GetActor());
			if (!TargetInterface)
				continue;
		}

		uint8 isValidTarget = IsValidTarget(TargetInterface, Targets, Exclusions);

		if (!isValidTarget)
			continue;


		if (TargetInterface->GetIsDead())
		{
			if (!TargetInterface->GetCanDeadShield())
				continue;
		}
		else 
		{
			uint8 HitsWeakness = 0;

			if (TargetInterface->GetHasWeakness())
			{
				TArray<FHitResult> WeaknessHitResults;

				Attacker->GetWorld()->SweepMultiByChannel(WeaknessHitResults, Position, Position, FQuat(), ECC_GameTraceChannel3, CollisionShape);

				for (FHitResult WeaknessHitResult : WeaknessHitResults)
					if (WeaknessHitResult.GetComponent() == TargetInterface->GetWeaknessComponent())
					{
						HitsWeakness = 1;
						break;
					}
			}

			FVector AttackDirection = HitResult.Location - Attacker->GetActorLocation();
			AttackDirection.Normalize();

			TargetInterface->WPTakeDamage(Damage, Penetration, HitsWeakness, StatusEffectNames, StatusEffectAmount,AttackDirection,HitResult.Location);
		}

		NumOfHit++;

	
	}


	return NumOfHit;
}

uint32 IWPAttackTargetInterface::SpherecastAttack(AActor* Attacker, FVector Position, float Radius, float Damage, float Penetration, TArray<IWPAttackTargetInterface*> Targets, TArray<IWPAttackTargetInterface*> Exclusions, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount)
{
	return ShapecastAttack(Attacker, FCollisionShape::MakeSphere(Radius),Position,Damage,Penetration,Targets,Exclusions,StatusEffectNames,StatusEffectAmount);
}

uint8 IWPAttackTargetInterface::IsValidTarget(IWPAttackTargetInterface* TargetInterface, TArray<IWPAttackTargetInterface*> Targets, TArray<IWPAttackTargetInterface*> Exclusions)
{
	uint8 isValidTarget;

	if (Targets.Num() == 0)
	{
		isValidTarget = 1;
		for (IWPAttackTargetInterface* Exclusion : Exclusions)
			if (Exclusion == TargetInterface)
			{
				isValidTarget = 0;
				break;
			}
	}
	else
	{
		isValidTarget = 0;

		for (IWPAttackTargetInterface* Target : Targets)
			if (Target == TargetInterface)
			{
				isValidTarget = 1;
				break;
			}
	}


	return isValidTarget;
}
