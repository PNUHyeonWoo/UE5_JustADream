// Fill out your copyright notice in the Description page of Project Settings.


#include "test/WPTestCubeActor.h"
#include <Components/BoxComponent.h>
#include "test/WPTestBoxSheildComponent.h"

// Sets default values
AWPTestCubeActor::AWPTestCubeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = BoxComp;
	WeaknessBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaknessBoxComp"));
	WeaknessBoxComp->SetupAttachment(BoxComp);
	BoxShieldComp = CreateDefaultSubobject<UWPTestBoxSheildComponent>(TEXT("ShieldBoxComp"));
	BoxShieldComp->SetupAttachment(BoxComp);

}

void AWPTestCubeActor::WPTakeDamage(float Damage, float Penetration, uint8 HitsWeakness, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount, FVector HitDirection, FVector HitPosition)
{
	IWPAttackTargetInterface::WPTakeDamage(Damage, Penetration, HitsWeakness, StatusEffectNames, StatusEffectAmount, HitDirection, HitPosition);
	UE_LOG(LogTemp, Log, TEXT("%s : HP=%f IsDead=%d"), *(GetFName().ToString()),CurrentHP,IsDead);
}

void AWPTestCubeActor::Die()
{
	UE_LOG(LogTemp, Log, TEXT("%s : Call Die"), *(GetFName().ToString()));
}

// Called when the game starts or when spawned
void AWPTestCubeActor::BeginPlay()
{
	Super::BeginPlay();

	Defence = 1;
	SetWeaknessComponent(WeaknessBoxComp);
	CanDeadShield = 0;

	for (AActor* a : Targets)
		TargetsI.Add(Cast<IWPAttackTargetInterface>(a));

	for (AActor* a : Ex)
		ExI.Add(Cast<IWPAttackTargetInterface>(a));


	if(isAttacker)
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AWPTestCubeActor::Attack, 1, true);
	
}

// Called every frame
void AWPTestCubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWPTestCubeActor::Attack()
{;
	IWPAttackTargetInterface::SpherecastAttack(this, FVector(0, 0, 40), 275, 50, 0, TargetsI, ExI);
	//IWPAttackTargetInterface::RaycastAttack(this, FVector(0, 0, 40), FVector(1000, 0, 40),50, 4,TargetsI,ExI);
	//IWPAttackTargetInterface::RaycastAttack(this, FVector(0, 0, 40), FVector(1000, 0, 40), 50, 0);
	//IWPAttackTargetInterface::RaycastAttack(this, FVector(0, 0, 40), FVector(1000, 0, 40), 50, 0);
	DrawDebugLine(GetWorld(), FVector(0, 0, 40), FVector(175, 0, 40), FColor::Emerald, true, 0.3f, 0, 10);

}

