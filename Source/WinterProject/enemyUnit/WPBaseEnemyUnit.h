// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "interface/WPAttackTargetInterface.h"
#include "GameFramework/Character.h"
#include "WPBaseEnemyUnit.generated.h"

class UBTTaskNode;
class AWPBaseEnemyUnitAIController;
class AWPPatrolSpline;
class UNiagaraSystem;
class USoundBase;
class UAudioComponent;
class AWPFireball;
class AWPAOE;

UENUM(BlueprintType)
enum class EIdleTask : uint8 {
	Idle UMETA(DisplayName = "Idle"),
	Rotate UMETA(DisplayName = "Rotate"),
	RandomMove UMETA(DisplayName = "Random Move"),
	Patrol UMETA(DisplayName = "Patrol")
};

UENUM(BlueprintType)
enum class EAlertTask : uint8 {
	Alert UMETA(DisplayName = "Alert"),
	Chase UMETA(DisplayName = "Chase"),
	TrackPlayer UMETA(DisplayName = "Track Player")
};

UENUM(BlueprintType)
enum class EUnableAttackTask : uint8
{
	Charge UMETA(DisplayName = "Charge"),
	Attack UMETA(DisplayName = "Attack"),
	Guard UMETA(DisplayName = "Guard"),
	Wander UMETA(DisplayName = "Wander")
};

USTRUCT(BlueprintType)
struct FAttackValidationMethod
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseRaycast;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackValidationTime;
};

UCLASS()
class WINTERPROJECT_API AWPBaseEnemyUnit : public ACharacter, public IWPAttackTargetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWPBaseEnemyUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AttackStart();
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	virtual void Die() override;

	virtual void WPTakeDamage(float Damage, float Penetration, uint8 HitsWeakness, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount, FVector HitDirection, FVector HitPosition) override;

	UPROPERTY(EditAnywhere)
	float I_MaxHP;
	UPROPERTY(EditAnywhere)
	float I_CurrentHP;
	UPROPERTY(EditAnywhere)
	float I_Defence;

	UPROPERTY(EditAnywhere)
	float GoldAmount;

	UPROPERTY(EditAnywhere)
	bool bIsArmored;

	UPROPERTY(EditAnywhere)
	int AttackToPerform;

	UPROPERTY(EditAnywhere)
	float WalkSpeed = 150.0f;

	UPROPERTY(EditAnywhere)
	float RunSpeed = 250.0f;

	UPROPERTY(EditAnywhere)
	float RotateSpeed = 200.0f;

	UPROPERTY(EditAnywhere)
	float BleedingAmount;

	UPROPERTY(EditAnywhere)
	float PoisonAmount;

	UPROPERTY(EditAnywhere)
	EIdleTask IdleTask;

	UPROPERTY(EditAnywhere)
	EAlertTask AlertTask;

	UPROPERTY(EditAnywhere)
	EUnableAttackTask UnableAttackTask;

	UPROPERTY(VisibleAnywhere)
	bool bMoveWhenOutOfRange;

	UPROPERTY(EditAnywhere)
	float AlertnessLevel = 0.0f;

	// 안쓰고 있음
	UPROPERTY(VisibleAnywhere)
	FVector PredictedPlayerLocation;

	// 위치 갱신 시간
	UPROPERTY(VisibleAnywhere)
	float LastPredictedPlayerLocationTime;

	// 위치 인식 시간
	UPROPERTY(EditAnywhere)
	float SightLostTrackDuration;

	// 벽 너머로 넘어간 순간부터 카운트
	UPROPERTY(EditAnywhere)
	float TimeSinceLastSighted;

	UPROPERTY(EditAnywhere)
	float Location;

	UPROPERTY(EditAnywhere)
	float AttackRange;

	UPROPERTY(EditAnywhere)
	UBTTaskNode* AttackTaskNode;

	UPROPERTY(EditAnywhere)
	// bIsAttackValid는 true, AttackValidationTime은 0.0f로 초기화
	TArray<FAttackValidationMethod> AttackValidationMethods = { {true, 0.0f} };

	UPROPERTY(EditAnywhere)
	bool bRotateWhenAlerted;

	UPROPERTY(EditAnywhere)
	uint32 IconIndex;

	UPROPERTY(EditAnywhere)
	float ActiveRange = 5000.f;

	UPROPERTY(EditAnywhere)
	int SplineIndex;

	UPROPERTY(VisibleAnywhere)
	int PatrolIndex;

	UPROPERTY(EditAnywhere)
	AWPPatrolSpline* PatrolSpline;

	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* WallBlood;

	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* Blood;

	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* BackBlood;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> RayEffectActor;

	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ArmorSound;

	UPROPERTY(EditAnywhere)
	USoundBase* AttackSound;

	UPROPERTY(EditAnywhere)
	USoundBase* DieSound;

	UPROPERTY(EditAnywhere, Category = "FX")
	TSubclassOf<AWPFireball> FireballClass;

	UPROPERTY(EditAnywhere, Category = "FX")
	TSubclassOf<AWPAOE> AOEClass;

	UPROPERTY(EditAnywhere)
	class UAudioComponent* AliveSoundComponent;
};
