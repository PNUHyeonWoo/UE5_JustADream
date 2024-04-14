// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "WPBTD_Alert.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPBTD_Alert : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UWPBTD_Alert();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float MinAlertLevel = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float MaxAlertLevel = 100.0f;
};
