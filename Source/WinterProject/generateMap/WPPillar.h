// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WPPillar.generated.h"

class ULevel;

UCLASS()
class WINTERPROJECT_API AWPPillar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWPPillar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* CollisionComponent;

	UPROPERTY(EditAnywhere)
	float Radius = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	ULevel* OwnerLevel;

};
