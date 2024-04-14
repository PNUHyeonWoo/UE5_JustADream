// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WPPath.generated.h"

UCLASS()
class WINTERPROJECT_API AWPPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWPPath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void LiftPathFog();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PathMesh;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* ClosetPositionComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* Wall1PositionComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* Wall2PositionComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* Door1PositionComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* Door2PositionComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ClosetActorClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> WallActorClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DoorActorClass;


	UPROPERTY()
	uint8 isOverlapStart;

	UPROPERTY(VisibleAnywhere)
	FVector2D PathIndex;

};
