// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/WPBaseEnemyUnit.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "enemyUnit/WPGhoulEnemyUnit.h"
#include "enemyUnit/AnimInstance/WPGhoulAnimInstance.h"
#include "enemyUnit/WPPillarArcher.h"
#include "enemyUnit/WPBaseEnemyUnitAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// 테스트용
#include "DrawDebugHelpers.h"
#include "player/WPPlayerActor.h"
#include "Kismet/GameplayStatics.h"
#include "enemyUnit/AnimInstance/WPBipedalAnimInstance.h"

#include "Engine/LevelBounds.h"
#include "generateMap/WPPatrolSpline.h"
#include "interaction/WPDropGold.h"

#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"

#include "enemyUnit/FX/WPFireball.h"
#include "enemyUnit/FX/WPAOE.h"

#include "enemyUnit/WPWarriorEnemyUnit.h"

// Sets default values
AWPBaseEnemyUnit::AWPBaseEnemyUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AliveSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AliveSoundComponent"));
	AliveSoundComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWPBaseEnemyUnit::BeginPlay()
{
	Super::BeginPlay();

	SetMaxHP(I_MaxHP);
	SetCurrentHP(I_CurrentHP);
	SetDefence(I_Defence);

	// MovementComponent의 속도를 WalkSpeed로 설정한다.
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// RotateRate.Yaw를 RotateSpeed로 설정한다.
	GetCharacterMovement()->RotationRate.Yaw = RotateSpeed;

	// bUseControllerDesiredRotation = true로 설정한다.
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	// 컨트롤러 회전 yaw 사용을 false
	bUseControllerRotationYaw = false;

	ALevelBounds* LevelBoundsActor = GetLevel()->LevelBoundsActor.Get();

	// LevelBoundsActor가 nullptr이 아니라면
	if (LevelBoundsActor == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("LevelBoundsActor is nullptr"));
		return;
	}

	FBox Bounds = LevelBoundsActor->GetComponentsBoundingBox(true);

	// Bounds와 오버랩되는 WPPatrolSpline를 찾는다.
	// Bounds의 IsInside 함수를 사용하여 WPPatrolSpline의 위치를 통해 내부에 존재하는 WPPatrolSpline를 찾는다.
	// 그리고 AWPBaseEnemyUnit의 SplineIndex와 WPPatrolSpline의 SplineIndex가 같은 것을 찾아서 WPPatrolSpline에 넣는다.

	TArray<AActor*> OverlappingActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWPPatrolSpline::StaticClass(), OverlappingActors);

for (AActor* Actor : OverlappingActors) {
		AWPPatrolSpline* WPPatrolSpline = Cast<AWPPatrolSpline>(Actor);
		if (WPPatrolSpline == nullptr) {
			continue;
		}

		if (Bounds.IsInside(WPPatrolSpline->GetActorLocation())) {

			if (WPPatrolSpline->SplineIndex == SplineIndex) {
				PatrolSpline = WPPatrolSpline;
			}
		}
	}

}

// Called every frame
void AWPBaseEnemyUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// AttackRange만큼 DebugDrawCircle을 그린다.
	DrawDebugCircle(GetWorld(), GetActorLocation(), AttackRange, 32, FColor::Red, false, -1.f, 0, 2.f, FVector(0.f, 1.f, 0.f), FVector(1.f, 0.f, 0.f), false);

}

// Called to bind functionality to input
void AWPBaseEnemyUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWPBaseEnemyUnit::AttackStart() {
}

void AWPBaseEnemyUnit::AttackEnd() {
}

void AWPBaseEnemyUnit::Die() {

	AliveSoundComponent->Stop();

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DieSound, GetActorLocation());

	AWPDropGold* DropGold;
	DropGold->CreateDropGold(GetWorld(), GoldAmount, GetActorLocation()+FVector(0,0,50));

	if (GetClass()->IsChildOf(AWPGhoulEnemyUnit::StaticClass())) {
		Cast<UWPGhoulAnimInstance>(GetMesh()->GetAnimInstance())->IsDead = 1;
	}
	else {
		Cast<UWPBipedalAnimInstance>(GetMesh()->GetAnimInstance())->IsDead = 1;
	}

	//// 컨트롤러를 가져와서 BT를 중지시킨다.
	//AAIController* AIController = Cast<AAIController>(GetController());
	//if (AIController == nullptr) {
	//	UE_LOG(LogTemp, Error, TEXT("AIController is nullptr"));
	//	return;
	//}

	DetachFromControllerPendingDestroy();

	/*GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);

	UE_LOG(LogTemp, Warning, TEXT("AWPBaseEnemyUnit::Die"));
}

void AWPBaseEnemyUnit::WPTakeDamage(float Damage, float Penetration, uint8 HitsWeakness, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount, FVector HitDirection, FVector HitPosition) {

	if(!(WallBlood && Blood && BackBlood && HitSound)) {
		UE_LOG(LogTemp, Error, TEXT("WallBlood or Blood or BackBlood or HitSound is nullptr"));
		return;
	}

	AWPBaseEnemyUnitAIController* AIController = Cast<AWPBaseEnemyUnitAIController>(GetController());
	if (AIController == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AIController is nullptr"));
		return;
	}

	// SphereOverlapActors
	TArray<AActor*> SphereOverlapActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> SphereOverlapActorsFilter;
	SphereOverlapActorsFilter.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	// 주변의 적들이 알아채는 범위
	float NoticeRange = 600.0f;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), NoticeRange, SphereOverlapActorsFilter, TSubclassOf<AWPBaseEnemyUnit>(), TArray<AActor*>(), SphereOverlapActors);

	for (AActor* Actor : SphereOverlapActors) {
		AWPBaseEnemyUnit* Unit = Cast<AWPBaseEnemyUnit>(Actor);
		if (Unit == nullptr) {
			continue;
		}
		AWPBaseEnemyUnitAIController* UnitAIController = Cast<AWPBaseEnemyUnitAIController>(Unit->GetController());
		UnitAIController->GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Player")), GetWorld()->GetFirstPlayerController()->GetPawn());
		Unit->AlertnessLevel = 400.0f;
	}


	// 갑옷을 입었다면
	if (bIsArmored) {
		// 이 GetDefence()가 Penetration보다 높다면 WallBlood만
		// 같다면 WallBlood와 Blood 생성
		// 작다면 WallBlood와 Blood, BackBlood 모두 생성
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ArmorSound, GetActorLocation());

		if (GetDefence() > Penetration) {
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WallBlood, HitPosition); 

		}
		else if (GetDefence() == Penetration) {
			// WallBlood와 Blood 생성
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WallBlood, HitPosition);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Blood, HitPosition);

		}
		else {
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WallBlood, HitPosition);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Blood, HitPosition);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BackBlood, HitPosition, HitDirection.Rotation());
		}
	}
	// 갑옷을 입지 않았다면
	else {
		// 이 GetDefence()가 Penetration보다 높다면 Blood만
		// 작다면 Blood, BackBlood 모두 생성
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());

		if (GetDefence() >= Penetration) {
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Blood, HitPosition);
		}
		else {
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Blood, HitPosition);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BackBlood, HitPosition, HitDirection.Rotation());
		}
	}

	IWPAttackTargetInterface::WPTakeDamage(Damage, Penetration, HitsWeakness, StatusEffectNames, StatusEffectAmount, HitDirection, HitPosition);

}