// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/WPWarriorEnemyUnit.h"
#include "enemyUnit/AnimInstance/WPBipedalAnimInstance.h"

AWPWarriorEnemyUnit::AWPWarriorEnemyUnit()
{
	// PrimaryActorTick.bCanEverTick = true;

	UnableAttackTask = EUnableAttackTask::Charge;
}

void AWPWarriorEnemyUnit::AttackStart()
{
	Super::AttackStart();

	/*TArray<IWPAttackTargetInterface*> Exclusions;
	Exclusions.Add(this);

	RaycastAttack(this, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * AttackRange, AttackDamageAndPierce[AttackToPerform].Damage, AttackDamageAndPierce[AttackToPerform].Pierce, TArray<IWPAttackTargetInterface*>(), Exclusions);*/
}

void AWPWarriorEnemyUnit::AttackEnd()
{
	Super::AttackEnd();
}