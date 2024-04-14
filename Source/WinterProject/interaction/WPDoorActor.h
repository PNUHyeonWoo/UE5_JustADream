// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "struct/WPUndyingAttackTargetActor.h"
#include "interface/WPInteractionInterface.h"
#include "WPDoorActor.generated.h"

/**
 * 
 */
UCLASS()
class WINTERPROJECT_API AWPDoorActor : public AWPUndyingAttackTargetActor, public IWPInteractionInterface
{
	GENERATED_BODY()
public:
	AWPDoorActor();
	virtual void Interact(class AWPPlayerActor* PlayerActor) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyHit
	(
		class UPrimitiveComponent* MyComp,
		AActor* Other,
		class UPrimitiveComponent* OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult& Hit
	) override;

	float GetDoorToActorDirection(AActor* Target);

protected:
	UPROPERTY(EditAnyWhere)
	TObjectPtr<USceneComponent> BaseComponent;

	UPROPERTY(EditAnyWhere)
	TObjectPtr<USceneComponent> AxisComponent;
	UPROPERTY(EditAnyWhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	UPROPERTY(EditAnyWhere)
	TObjectPtr<UStaticMeshComponent> SightComponent;
	UPROPERTY()
	float RotationDirection;
	UPROPERTY(EditAnyWhere)
	float RotationAngle;
	UPROPERTY(EditAnyWhere)
	float RotationSpeed;
	UPROPERTY(EditAnyWhere)
	uint8 bIsOpen : 1;

	UPROPERTY(EditAnyWhere)
	TObjectPtr<USoundWave> OpenSound;
	UPROPERTY(EditAnyWhere)
	TObjectPtr<USoundWave> CloseSound;

	UPROPERTY(EditAnyWhere,BlueprintReadWrite, Category = "NavMesh")
	TObjectPtr<UActorComponent> NavMeshComponent;
	
};
