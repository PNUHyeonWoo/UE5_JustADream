// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WPArenaMapGenerator.generated.h"

class UWPArena;
class AWPPath;
class AWPPillar;

DECLARE_MULTICAST_DELEGATE(FOnAllLevelIsShown);

UCLASS()
class WINTERPROJECT_API AWPArenaMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWPArenaMapGenerator();

	FOnAllLevelIsShown Fuc_OnAllLevelIsShown;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int32 LoadedLevelsCount;

	UPROPERTY(EditAnywhere, Category = "Arena")
	int32 Floor = 1;

	// 1아레나 확률
	UPROPERTY(EditAnywhere, Category = "Arena")
	int32 ProbArena1 = 40;

	// 2아레나 확률
	UPROPERTY(EditAnywhere, Category = "Arena")
	int32 ProbArena2 = 40;

	// 4아레나 확률
	UPROPERTY(EditAnywhere, Category = "Arena")
	int32 ProbArena4 = 20;

	UPROPERTY(EditAnywhere, Category = "Arena")
	int32 NumArena1 = 0;

	UPROPERTY(EditAnywhere, Category = "Arena")
	int32 NumArena2 = 0;

	UPROPERTY(EditAnywhere, Category = "Arena")
	int32 NumArena4 = 0;

	UPROPERTY(VisibleAnywhere, Category = "Arena")
	UWPArena* DefaultArena;

	UPROPERTY(EditAnywhere, Category = "Arena")
	int32 ArenaDistance = 5000;

	UPROPERTY()
	FString BossName;

private:
	// 배치된 각 아레나들 저장용
	UPROPERTY(VisibleAnywhere, Category = "Arena")
	TArray<UWPArena*> Arenas;
	// 아레나들의 5x5 배치 저장용
	
	TArray<TArray<UWPArena*>> ArenaPlacement;

	UPROPERTY(VisibleAnywhere, Category = "Arena")
	TArray<FVector2D> PathPositions;

	UPROPERTY(EditAnywhere, Category = "Path")
	TSubclassOf<AWPPath> PathClass;

	uint8 bDidCallGeneratePath = 0;

	FString GetRandomLevelName(FString Path);

	// 5X5 그리드에 아레나를 배치하는 메소드
	void GenerateArenas();
	// 아레나를 연결하는 메소드
	void ConnectArenasMST();
	// 추가적인 연결을 위한 메소드
	void ConnectAdditionalPaths();

	bool CanConnectArenas(UWPArena* Arena1, UWPArena* Arena2);

	void ArenaPlacementInit();

	void PlaceArena4(int i, int j);
	void PlaceArena2(int i, int j);
	void PlaceArena1(int i, int j);

	void GeneratePaths();

	UFUNCTION()
	void OnLevelShown();

	void OnAllLevelsShown();
};
