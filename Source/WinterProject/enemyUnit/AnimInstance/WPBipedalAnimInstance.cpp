// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/AnimInstance/WPBipedalAnimInstance.h"
#include "enemyUnit/WPBipedalEnemyUnit.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "enemyUnit/WPWarriorEnemyUnit.h"
#include "player/WPPlayerActor.h"
#include "Kismet/GameplayStatics.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "enemyUnit/FX/WPFireball.h"
#include "enemyUnit/FX/WPAOE.h"

UWPBipedalAnimInstance::UWPBipedalAnimInstance()
{
}

void UWPBipedalAnimInstance::PostInitProperties() {
	Super::PostInitProperties();

	// IdleUpperBodyAnimClip = LoadObject<UAnimSequence>(nullptr, TEXT("/Script/Engine.AnimSequence'/Game/EnemyUnit/Assets/OrcShield/OrcShield_Idle_Anim.OrcShield_Idle_Anim'"));
	// AlertUpperBodyAnimClip = LoadObject<UAnimSequence>(nullptr, TEXT("/Script/Engine.AnimSequence'/Game/EnemyUnit/Assets/Zombie/Animation/Zombie_Anim_Tpose.Zombie_Anim_Tpose'"));
}

void UWPBipedalAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();
}

void UWPBipedalAnimInstance::NativeBeginPlay() {
	Super::NativeBeginPlay();

	EnemyUnit = Cast<AWPBipedalEnemyUnit>(TryGetPawnOwner());
	if (!EnemyUnit)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyUnit is nullptr"));
		return;
	}

	// EnemyUnit의 AIController의 BlackboardComponent를 가져온다.
	AIController = Cast<AAIController>(EnemyUnit->GetController());
	// BlackboardComponent를 가져온다.
	BlackboardComponent = AIController->GetBlackboardComponent();

	IdleUpperBodyAnimClip = EnemyUnit->IdleUpperBodyAnimClip;
	AlertUpperBodyAnimClip = EnemyUnit->AlertUpperBodyAnimClip;
	AttackAnimClips = EnemyUnit->AttackAnimClips;
}

void UWPBipedalAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!EnemyUnit)
	{
		return;
	}

	FVector2D Velocity2D(EnemyUnit->GetVelocity());
	FVector2D ControlRotation2D(FRotationMatrix(EnemyUnit->GetActorRotation()).GetUnitAxis(EAxis::X));

	Speed = Velocity2D.Length() / EnemyUnit->RunSpeed;

	Velocity2D.Normalize();
	ControlRotation2D.Normalize();

	if (Speed > 0.01f) {
		float Angle = acosf(Velocity2D.Dot(ControlRotation2D)) * 180.0f / PI;
		float Det = ControlRotation2D.X * Velocity2D.Y - ControlRotation2D.Y * Velocity2D.X;
		if (Det > 0) {
			Angle = 360 - Angle;
		}
		RelativeMovingAngle = Angle;
	}

	AlertnessLevel = EnemyUnit->AlertnessLevel;
	AttackToPerform = EnemyUnit->AttackToPerform;
}

void UWPBipedalAnimInstance::AnimNotify_OnAttackEnd() {
	bIsAttacking = false;

	EnemyUnit->AttackEnd();
}

void UWPBipedalAnimInstance::AnimNotify_OnDealDamageShapecast() {

	// BlackboardComponent에 IsAttackStart를 true로 설정한다.
	BlackboardComponent->SetValueAsBool(TEXT("IsAttackStart"), true);
	FVector PredictedPlayerLocation = BlackboardComponent->GetValueAsVector(TEXT("PredictedPlayerLocation"));

	if (EnemyUnit->AttackSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnemyUnit->AttackSound, EnemyUnit->GetActorLocation());
	}

	TArray<IWPAttackTargetInterface*> Targets;

	AWPPlayerActor* Player = Cast<AWPPlayerActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Targets.Add(Player);

	float Damage = EnemyUnit->AttackDamageAndPierce[EnemyUnit->AttackToPerform].Damage;
	float Pierce = EnemyUnit->AttackDamageAndPierce[EnemyUnit->AttackToPerform].Pierce;

	// EnemyUnit->ShapecastAttack(EnemyUnit, FCollisionShape::MakeSphere(EnemyUnit->AttackRange), , Damage, Pierce, Targets, TArray<IWPAttackTargetInterface*>());
	// 본인의 위치에서 전방으로 100.f만큼의 거리에 대해 공격한다.
	EnemyUnit->ShapecastAttack(EnemyUnit, FCollisionShape::MakeSphere(100.f), EnemyUnit->GetActorLocation() + EnemyUnit->GetActorForwardVector() * 100.f, Damage, Pierce, Targets, TArray<IWPAttackTargetInterface*>(), { TEXT("bleeding"),TEXT("poison") }, {Damage*EnemyUnit->BleedingAmount,EnemyUnit->PoisonAmount});
}

void UWPBipedalAnimInstance::AnimNotify_OnDealDamageRaycast() {

	// BlackboardComponent에 IsAttackStart를 true로 설정한다.
	BlackboardComponent->SetValueAsBool(TEXT("IsAttackStart"), true);

	if (EnemyUnit->AttackSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnemyUnit->AttackSound, EnemyUnit->GetActorLocation());
	}

	TArray<IWPAttackTargetInterface*> Exclusions;

	// 플레이어를 찾아서 데미지를 주는 함수 호출
	AWPPlayerActor* Player = Cast<AWPPlayerActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Exclusions.Add(EnemyUnit);

	float Damage = EnemyUnit->AttackDamageAndPierce[EnemyUnit->AttackToPerform].Damage;
	float Pierce = EnemyUnit->AttackDamageAndPierce[EnemyUnit->AttackToPerform].Pierce;

	if (Player) {

		FVector LastHitPosition;

		EnemyUnit->RaycastAttack(EnemyUnit, EnemyUnit->GetActorLocation(), EnemyUnit->GetActorLocation() + EnemyUnit->GetActorForwardVector() * 2000.f, Damage, Pierce, TArray<IWPAttackTargetInterface*>(), Exclusions , { TEXT("bleeding"),TEXT("poison") }, { Damage * EnemyUnit->BleedingAmount,EnemyUnit->PoisonAmount },&LastHitPosition);
		//DrawDebugLine(GetWorld(), EnemyUnit->GetActorLocation(), EnemyUnit->GetActorLocation() + EnemyUnit->GetActorForwardVector() * 2000.f, FColor::Yellow, false, 0.02f);
		if (EnemyUnit->RayEffectActor)
		{
			GetWorld()->SpawnActor<AActor>(EnemyUnit->RayEffectActor, FTransform(EnemyUnit->GetActorForwardVector().Rotation(), (EnemyUnit->GetActorLocation() + LastHitPosition ) / 2))->SetActorScale3D(FVector((EnemyUnit->GetActorLocation() - LastHitPosition).Length()/100, 0.015, 0.015));
		}

	}
}

void UWPBipedalAnimInstance::AnimNotify_OnSpellCast() {
	// BlackboardComponent에 IsAttackStart를 true로 설정한다.
	BlackboardComponent->SetValueAsBool(TEXT("IsAttackStart"), true);

	// EnemyUnit->FireballClass를 사용하여 Fireball을 생성하며, Owner를 EnemyUnit으로 설정한다.
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = EnemyUnit;
	SpawnParams.Instigator = EnemyUnit;

	float Damage = EnemyUnit->AttackDamageAndPierce[EnemyUnit->AttackToPerform].Damage;
	float Pierce = EnemyUnit->AttackDamageAndPierce[EnemyUnit->AttackToPerform].Pierce;

	// EnemyUnit->AttackToPerform이 0이면 Fireball, 1이면 AOE를 생성한다.
	if (EnemyUnit->AttackToPerform == 0) {
		AWPFireball* Fireball = GetWorld()->SpawnActor<AWPFireball>(EnemyUnit->FireballClass, EnemyUnit->GetActorLocation() + EnemyUnit->GetActorForwardVector() * 100.f, EnemyUnit->GetActorRotation(), SpawnParams);
	}
	else if (EnemyUnit->AttackToPerform == 1) {
		FVector PredictedPlayerLocation = BlackboardComponent->GetValueAsVector(TEXT("PredictedPlayerLocation"));
		AWPAOE* AOE = GetWorld()->SpawnActor<AWPAOE>(EnemyUnit->AOEClass, PredictedPlayerLocation, EnemyUnit->GetActorRotation(), SpawnParams);
	}

}