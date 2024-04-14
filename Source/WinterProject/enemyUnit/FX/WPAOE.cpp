// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/FX/WPAOE.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "player/WPPlayerActor.h"
#include "enemyUnit/WPBipedalEnemyUnit.h"
#include "Components/AudioComponent.h"

// Sets default values
AWPAOE::AWPAOE()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	FX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CastFX"));
	FX->SetupAttachment(Root);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(FX);

	StormAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("StormSound"));
	StormAudio->SetupAttachment(Root);
	StormAudio->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AWPAOE::BeginPlay()
{
	Super::BeginPlay();
	
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AWPAOE::OnBeginOverlap);
	Capsule->OnComponentEndOverlap.AddDynamic(this, &AWPAOE::OnEndOverlap);

	EnemyPawn = Cast<AWPBipedalEnemyUnit>(GetOwner());
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AWPAOE::SpawnStorm, CastTime, false);

}

// Called every frame
void AWPAOE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWPAOE::Init(float InDamage, float InPierce) {
	this->Damage = InDamage;
	this->Pierce = InPierce;
}

void AWPAOE::SpawnStorm()
{
	// FX의 이펙트를 StormFX로 변경
	FX->SetTemplate(StormFX);

	// 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("Spawn Storm"));

	// StormAudio를 재생
	StormAudio->Play();

	// Capsule의 충돌을 활성화
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	FVector StartScale = FVector(0.f);
	FX->SetWorldScale3D(StartScale);

	GetWorld()->GetTimerManager().SetTimer(ScaleTimerHandle, this, &AWPAOE::IncreaseStormScale, 0.1f, true);

	// LifeTime이 끝나면 파티클을 제거
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AWPAOE::DestroyStorm, LifeTime, false);

}

void AWPAOE::IncreaseStormScale()
{
	FVector CurrentScale = FX->GetComponentScale();
	FVector TargetScale = FVector(1.f);
	FVector DeltaScale = (TargetScale - CurrentScale) * 0.1f;
	
	if (CurrentScale.Equals(TargetScale, 0.01f)) {
		FX->SetWorldScale3D(TargetScale);
		GetWorld()->GetTimerManager().ClearTimer(ScaleTimerHandle);
	}
	else {
		FX->SetWorldScale3D(CurrentScale + DeltaScale);
	}
}

void AWPAOE::DecreaseStormScale()
{
	FVector CurrentScale = FX->GetComponentScale();
	FVector TargetScale = FVector(0.f);
	FVector DeltaScale = (TargetScale - CurrentScale) * 0.1f;

	if (CurrentScale.Equals(TargetScale, 0.01f)) {
		FX->SetWorldScale3D(TargetScale);
		GetWorld()->GetTimerManager().ClearTimer(ScaleTimerHandle);
	}
	else {
		FX->SetWorldScale3D(CurrentScale + DeltaScale);
	}
}

void AWPAOE::DestroyStorm()
{
	GetWorld()->GetTimerManager().SetTimer(ScaleTimerHandle, this, &AWPAOE::DecreaseStormScale, 0.1f, true, 0.0f);

	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &AWPAOE::FinalizeStorm, 1.0f, false);;
}

void AWPAOE::FinalizeStorm()
{
	StormAudio->Stop();
	FX->Deactivate();
	Destroy();
}

void AWPAOE::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	// 대상이 플레이어인지 확인
	if (OtherActor->ActorHasTag("Player")) {
		
		GetWorld()->GetTimerManager().SetTimer(DealTimer, this, &AWPAOE::DealDamage, 0.5f, true);
	}
}

void AWPAOE::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	// 대상이 플레이어인지 확인
	if (OtherActor->ActorHasTag("Player")) {
		// 데미지를 입히는 타이머를 제거
		GetWorld()->GetTimerManager().ClearTimer(DealTimer);
	}
}

void AWPAOE::DealDamage() {
	AWPPlayerActor* Player = Cast<AWPPlayerActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	TArray<IWPAttackTargetInterface*> Targets = { Player };

	// 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("Deal Damage"));

	EnemyPawn->ShapecastAttack(EnemyPawn, Capsule->GetCollisionShape(), GetActorLocation(), Damage, Pierce, Targets, TArray<IWPAttackTargetInterface*>());

}