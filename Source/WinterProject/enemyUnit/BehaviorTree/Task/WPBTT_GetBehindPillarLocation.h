// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "WPBTT_GetBehindPillarLocation.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPBTT_GetBehindPillarLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UWPBTT_GetBehindPillarLocation();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	float Angle = 180.0f;

	UPROPERTY(EditAnywhere)
	bool bRandomSign = false;
};
