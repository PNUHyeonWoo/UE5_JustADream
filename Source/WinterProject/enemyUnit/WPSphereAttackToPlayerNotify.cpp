// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/WPSphereAttackToPlayerNotify.h"
#include "interface/WPAttackTargetInterface.h"

void UWPSphereAttackToPlayerNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* Attacker = MeshComp->GetOwner();

	if (!Attacker)
		return;

	FVector AttackPosition = Attacker->GetActorLocation() + Attacker->GetActorRotation().RotateVector(Position);

	if (!Attacker->GetWorld())
		return;

	APlayerController* PC = Attacker->GetWorld()->GetFirstPlayerController();

	if (PC)
	{
		APawn* PP = PC->GetPawn();

		if (PP)
		{
			IWPAttackTargetInterface* PlayerAttackTarget = Cast<IWPAttackTargetInterface>(PP);

			if (PlayerAttackTarget)
				IWPAttackTargetInterface::SpherecastAttack(Attacker, AttackPosition, Radius, Damage, Penetration, { PlayerAttackTarget }, {}, StatusEffectNames, StatusEffectAmounts);

			DrawDebugSphere(Attacker->GetWorld(), AttackPosition, Radius, 0, FColor::Red);
		}
	}

}
