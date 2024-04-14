// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/Boss/WPSwordMasterBoss.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "enemyUnit/Boss/WPSwordMasterAnimInstance.h"
#include "Animation/AnimNode_StateMachine.h"
#include <NiagaraFunctionLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "Components/AudioComponent.h"
#include "interaction/WPIronBarDoorActor.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values
AWPSwordMasterBoss::AWPSwordMasterBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(GetMesh());
}

void AWPSwordMasterBoss::WPTakeDamage(float Damage, float Penetration, uint8 HitsWeakness, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount, FVector HitDirection, FVector HitPosition)
{
	GetCharacterMovement()->StopMovementImmediately();
	CumulativeKnockback += HitDirection * Damage * 10;
	LaunchCharacter(CumulativeKnockback, true, true);

	UWPSwordMasterAnimInstance* AnimInstance = Cast<UWPSwordMasterAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->Parrying = 1 - AnimInstance->Parrying;

	AnimInstance->GoToParrying = 1;

	SoundIndex = (SoundIndex + 1) % HitSounds.Num();

	UGameplayStatics::PlaySoundAtLocation(this, HitSounds[SoundIndex], GetActorLocation(),Damage < 40 ? 0.75 : 1);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, HitPosition, (-HitDirection).Rotation());


}

void AWPSwordMasterBoss::Die()
{
	Cast<AAIController>(GetController())->StopMovement();

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);

	GetMesh()->AddTorqueInRadians(FVector(3140000, 3140000, 3140000));
	AudioComp->Play();
	

	if (IronBarDoor)
		IronBarDoor->SetDoorOpen(true);
}

void AWPSwordMasterBoss::ActivateBoss()
{
	bActivate = 1;
}

// Called when the game starts or when spawned
void AWPSwordMasterBoss::BeginPlay()
{
	Super::BeginPlay();

	Defence = 10;
	
}

// Called every frame
void AWPSwordMasterBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsDead && GetActorLocation().Z < 0)
	{
		IsDead = 1;
		Die();
	}

	if(bActivate && !IsDead)
		Cast<AAIController>(GetController())->MoveToActor(GetWorld()->GetFirstPlayerController()->GetPawn());

	CumulativeKnockback = FVector(0,0,0);

	if (GetActorLocation().Z < -100000)
	{
		GetCharacterMovement()->GravityScale = 0;
		GetCharacterMovement()->Velocity = FVector(0,0,0);
		GetMesh()->SetSimulatePhysics(false);
	}

	FName NowState = GetMesh()->GetAnimInstance()->GetStateMachineInstanceFromName(TEXT("Upper"))->GetCurrentStateName();

	if (!IsDead && NowState == TEXT("Attack"))
	{
		FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		PlayerLocation.Z = 0;
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation() - FVector(0, 0, GetActorLocation().Z), PlayerLocation));
	}


}

// Called to bind functionality to input
void AWPSwordMasterBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

