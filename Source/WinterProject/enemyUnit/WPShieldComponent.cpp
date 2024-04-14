// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/WPShieldComponent.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include <NiagaraFunctionLibrary.h>

UWPShieldComponent::UWPShieldComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	
}

// PostInitProperties()
void UWPShieldComponent::PostLoad()
{
	Super::PostLoad();

	//// 이 컴포넌트를 현재 스켈레탈메시의 ShieldSocket에 붙인다.
	//// 먼저 현재 컴포넌트의 Owner가 누구인지 확인한다.
	//AWPBaseEnemyUnit* Owner = Cast<AWPBaseEnemyUnit>(GetOwner());
	//USkeletalMeshComponent* OwnerMesh = Owner->GetMesh();

	//// 현재 컴포넌트를 Owner의 Mesh에 붙인다.
	//if (OwnerMesh)
	//	AttachToComponent(OwnerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "ShieldSocket");

}

void UWPShieldComponent::BeginPlay()
{
	Super::BeginPlay();

	// 이 컴포넌트를 현재 스켈레탈메시의 ShieldSocket에 붙인다.
	// 먼저 현재 컴포넌트의 Owner가 누구인지 확인한다.

	SetMaxHP(I_MaxHP);
	SetCurrentHP(I_CurrentHP);
	SetDefence(I_Defence);

	AWPBaseEnemyUnit* Owner = Cast<AWPBaseEnemyUnit>(GetOwner());
	USkeletalMeshComponent* OwnerMesh = Owner->GetMesh();

	// 현재 컴포넌트를 Owner의 Mesh에 붙인다.
	if (OwnerMesh)
		AttachToComponent(OwnerMesh, FAttachmentTransformRules::KeepWorldTransform, "LHandSocket");
}

void UWPShieldComponent::Die()
{
	// 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("ShieldComponent Die"));

	if (ShieldBreakSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShieldBreakSound, GetComponentLocation());

	// Destroy the shield
	DestroyComponent();
}

void UWPShieldComponent::WPTakeDamage(float Damage, float Penetration, uint8 HitsWeakness, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount, FVector HitDirection, FVector HitPosition) {

	IWPAttackTargetInterface::WPTakeDamage(Damage, Penetration, HitsWeakness, StatusEffectNames, StatusEffectAmount, HitDirection, HitPosition);

	if (ShieldHitSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShieldHitSound, GetOwner()->GetActorLocation());

	if (HitEffect)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, HitPosition,(-HitDirection).Rotation());
}