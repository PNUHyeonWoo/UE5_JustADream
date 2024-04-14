// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "WPBTT_AdjustAlertnessLevel.generated.h"

UENUM(BlueprintType)
enum class EAdjustType : uint8
{
	Addition UMETA(DisplayName = "Addition"),
	Substitution UMETA(DisplayName = "Substitution")
};

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API UWPBTT_AdjustAlertnessLevel : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UWPBTT_AdjustAlertnessLevel();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	EAdjustType AdjustType;

	UPROPERTY(EditAnywhere)
	float AdjustValue;
};
