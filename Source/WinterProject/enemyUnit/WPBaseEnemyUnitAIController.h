// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WPBaseEnemyUnitAIController.generated.h"

class UAIPerceptionComponent;
class AWPBaseEnemyUnit;

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API AWPBaseEnemyUnitAIController : public AAIController
{
	GENERATED_BODY()
public:
	AWPBaseEnemyUnitAIController();

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

protected:
	void BeginPlay() override;

	void OnPossess(APawn* InPawn) override;

	void Tick(float DeltaSeconds) override;

	void DecreaseAlert();

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	void SightLostTrack();

	UPROPERTY(EditAnywhere)
	AActor* TargetActor;

	UPROPERTY(EditAnywhere)
	AWPBaseEnemyUnit* EnemyPawn;

	FTimerHandle SightTimerHandle;

	FTimerHandle DecreaseAlertTimerHandle;

	UPROPERTY()
	FVector InitialLocation;

	bool bIsPlayerInSight;
};
