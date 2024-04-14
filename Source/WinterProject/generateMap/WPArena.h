// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Chaos/Pair.h"
#include "WPArena.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EArenaType : uint8 {
	ArenaDummy UMETA(DisplayName = "Arena Dummy"),
	Arena1 UMETA(DisplayName = "Arena 1"),
	ArenaHorizontal2 UMETA(DisplayName = "Arena Horizontal 2"),
	ArenaVertical2 UMETA(DisplayName = "Arena Vertical 2"),
	Arena4 UMETA(DisplayName = "Arena 4"),
};

UCLASS()
class WINTERPROJECT_API UWPArena : public UObject
{
	GENERATED_BODY()
	
public:

	void Init(int32 FArenaID, FString FArenaName, EArenaType FArenaType, FVector2D FArenaPosition);

	UPROPERTY(EditAnywhere, Category = "Arena")
	int32 ArenaID;

	UPROPERTY(EditAnywhere, Category = "Arena")
	FString ArenaName;

	UPROPERTY(EditAnywhere, Category = "Arena")
	EArenaType ArenaType;

	UPROPERTY(EditAnywhere, Category = "Arena")
	FVector2D ArenaPosition;

	UPROPERTY(EditAnywhere, Category = "Arena")
	TArray<FVector2D> ConnectedArenas;

	UPROPERTY(EditAnywhere, Category = "Arena")
	TArray<FVector2D> ArenaSide;

	UPROPERTY(VisibleAnywhere, Category = "Arena")
	TMap<uint32,uint32> NumOfIcons;

	UPROPERTY(VisibleAnywhere, Category = "Arena")
	TObjectPtr<class ULevelStreamingDynamic> LevelInstance;

};
