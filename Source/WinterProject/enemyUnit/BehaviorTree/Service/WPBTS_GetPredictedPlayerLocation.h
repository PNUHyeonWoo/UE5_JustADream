// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "WPBTS_GetPredictedPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPBTS_GetPredictedPlayerLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UWPBTS_GetPredictedPlayerLocation();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
