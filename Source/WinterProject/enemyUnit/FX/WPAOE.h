// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WPAOE.generated.h"

class UCapsuleComponent;
class AWPBipedalEnemyUnit;
class USoundBase;
class UAudioComponent;

UCLASS()
class WINTERPROJECT_API AWPAOE : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWPAOE();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Init(float InDamage, float InPierce);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void DealDamage();

	// 위험 장판을 생성하는 함수
	void SpawnStorm();

	void IncreaseStormScale();
	void DecreaseStormScale();

	// DestroyStorm
	void DestroyStorm();
	void FinalizeStorm();

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;

	UPROPERTY(EditAnywhere)
	float Pierce = 0.f;

	// 바닥에 위험 장판이 깔린후, n초후에 해당 위치에 폭풍이 생긴다.
	// n초 후를 정의할 변수
	UPROPERTY(EditAnywhere)
	float CastTime = 2.f;

	// 폭풍이 지속되는 시간
	UPROPERTY(EditAnywhere)
	float LifeTime = 2.f;

	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* FX;

	UPROPERTY(EditAnywhere)
	UParticleSystem* StormFX;

	UPROPERTY(EditAnywhere)
	UAudioComponent* StormAudio;

	FTimerHandle DealTimer;
	FTimerHandle Timer;
	FTimerHandle ScaleTimerHandle;

	UPROPERTY()
	AWPBipedalEnemyUnit* EnemyPawn;
};
