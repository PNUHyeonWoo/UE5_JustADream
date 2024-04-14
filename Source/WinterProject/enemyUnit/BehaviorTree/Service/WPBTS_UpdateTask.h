// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "WPBTS_UpdateTask.generated.h"

UENUM(BlueprintType)
enum class ETaskType : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Alert UMETA(DisplayName = "Alert"),
	UnableAttack UMETA(DisplayName = "UnableAttack")
};

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPBTS_UpdateTask : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UWPBTS_UpdateTask();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

	UPROPERTY(EditAnywhere)
	ETaskType TaskType;
};
