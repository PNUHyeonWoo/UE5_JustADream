// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "WPBTT_Run.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPBTT_Run : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UWPBTT_Run();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	bool IsRunning;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
