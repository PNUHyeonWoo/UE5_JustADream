// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyUnit/FX/WPFireball.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "player/WPPlayerActor.h"
#include "enemyUnit/WPBipedalEnemyUnit.h"
#include "struct/WPUndyingAttackTargetActor.h"
#include "Sound/SoundBase.h"

// Sets default values
AWPFireball::AWPFireball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 이 액터에 ky_shot 태그를 추가한다.
	Tags.Add("ky_shot");

	// DefaultSceneRoot를 Root로 설정한다.
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	FX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FX"));
	// 해당 FX를 Root에 붙인다.
	FX->SetupAttachment(Root);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(FX);
	Box->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AWPFireball::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &AWPFireball::OnBeginOverlap);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CastSound, GetActorLocation());
	
	// SpawnEmitterAtLocation을 사용하여 CastFX를 생성한다.
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CastFX, GetActorLocation(), GetActorRotation());
	
	// LifeTime이 지나면 Box의 콜리전을 비활성, FX를 비활성화하고 액터를 파괴한다.
	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AWPFireball::DestroyFireball, LifeTime, false);
}

// Called every frame
void AWPFireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalOffset(FVector(Speed * DeltaTime, 0.f, 0.f));

}

void AWPFireball::DestroyFireball() {
	//로그 출력
	UE_LOG(LogTemp, Warning, TEXT("DestroyFireball"));
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FX->Deactivate();
	Destroy();
}

void AWPFireball::Init(float InDamage, float InPierce) {
	this->Damage = InDamage;
	this->Pierce = InPierce;
}

// BoxComponent와 오버랩이 시작되면
void AWPFireball::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	// 태그에 ky_shot이 있다면 return
	if (OtherActor->ActorHasTag("ky_shot") || OtherActor == GetOwner()) {
		return;
	}

	if (!OtherActor->GetClass()->IsChildOf(AWPUndyingAttackTargetActor::StaticClass())) {
		if (OtherActor->GetClass()->ImplementsInterface(UWPAttackTargetInterface::StaticClass())) {
			// 해당 파이어볼을 발사한 주체가 없으면 에러남
			AWPBipedalEnemyUnit* EnemyPawn = Cast<AWPBipedalEnemyUnit>(GetOwner());

			TArray<IWPAttackTargetInterface*> Exclusions = { EnemyPawn };
			EnemyPawn->ShapecastAttack(EnemyPawn, FCollisionShape::MakeSphere(100.f), GetActorLocation(), Damage, Pierce, TArray<IWPAttackTargetInterface*>(), Exclusions);
		}
	}
	 
	// SpawnEmitterAtLocation을 사용하여 HitFX를 생성한다.
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFX, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());

	// 이 액터를 파괴한다.
	Destroy();
}