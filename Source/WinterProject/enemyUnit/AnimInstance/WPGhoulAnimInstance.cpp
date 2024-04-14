// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/AnimInstance/WPGhoulAnimInstance.h"
#include "enemyUnit/WPGhoulEnemyUnit.h"
#include "player/WPPlayerActor.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UWPGhoulAnimInstance::UWPGhoulAnimInstance()
{
	bIsAttacking = false;
	Speed = 0.0f;
}

void UWPGhoulAnimInstance::NativeBeginPlay() {
	Super::NativeBeginPlay();

	EnemyUnit = Cast<AWPGhoulEnemyUnit>(TryGetPawnOwner());
	if (!EnemyUnit)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyUnit is nullptr"));
		return;
	}

	AIController = Cast<AAIController>(EnemyUnit->GetController());
	BlackboardComponent = AIController->GetBlackboardComponent();
}

void UWPGhoulAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!EnemyUnit)
	{
		return;
	}

	FVector2D Velocity2D(EnemyUnit->GetVelocity());
	FVector2D ControlRotation2D(FRotationMatrix(EnemyUnit->GetActorRotation()).GetUnitAxis(EAxis::X));

	Speed = Velocity2D.Length() / EnemyUnit->RunSpeed;

	if (Speed > 0.01f) {
		float Angle = acosf(Velocity2D.Dot(ControlRotation2D)) * 180.0f / PI;
		float Det = ControlRotation2D.X * Velocity2D.Y - ControlRotation2D.Y * Velocity2D.X;
		if (Det > 0) {
			Angle = 360 - Angle;
		}
		RelativeMovingAngle = Angle;
	}
}

void UWPGhoulAnimInstance::AnimNotify_DashAttack() {

	// BlackboardComponent에 IsAttackStart를 true로 설정한다.
	BlackboardComponent->SetValueAsBool(TEXT("IsAttackStart"), true);

	FVector PredictedPlayerLocation = BlackboardComponent->GetValueAsVector(TEXT("PredictedPlayerLocation"));

	if (EnemyUnit->AttackSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnemyUnit->AttackSound, EnemyUnit->GetActorLocation());
	}

	TArray<IWPAttackTargetInterface*> Targets;
	
	AWPPlayerActor* Player = Cast<AWPPlayerActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Targets.Add(Player);

	EnemyUnit->ShapecastAttack(EnemyUnit, FCollisionShape::MakeSphere(100.f), EnemyUnit->GetActorLocation() + EnemyUnit->GetActorForwardVector() * 100.f, EnemyUnit->Damage, EnemyUnit->Pierce, Targets, TArray<IWPAttackTargetInterface*>() , { TEXT("bleeding"),TEXT("poison") }, { EnemyUnit->Damage * EnemyUnit->BleedingAmount,EnemyUnit->PoisonAmount });

}

void UWPGhoulAnimInstance::AnimNotify_OnAttackEnd() {
	bIsAttacking = false;

	EnemyUnit->AttackEnd();
}