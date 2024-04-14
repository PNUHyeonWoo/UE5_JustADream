// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WPPillarArcherAIController.generated.h"

class AWPPillarArcher;
class AWPPillar;
class AWPArenaMapGenerator;

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API AWPPillarArcherAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	AWPPillarArcherAIController();

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	void OnPossess(APawn* InPawn) override;

	// 모든 컴포넌트가 등록되었을때 호출되는 함수
	void PostRegisterAllComponents() override;

	void PostInitializeComponents() override;

	UFUNCTION()
	void FindNearestPillar();

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	UPROPERTY(EditAnywhere)
	AWPPillarArcher* EnemyPawn;

	AWPArenaMapGenerator* ArenaMapGenerator;
};
