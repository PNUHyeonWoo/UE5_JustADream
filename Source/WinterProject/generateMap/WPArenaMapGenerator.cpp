// Fill out your copyright notice in the Description page of Project Settings.


#include "generateMap/WPArenaMapGenerator.h"
#include "generateMap/WPArena.h"
#include "Engine/ObjectLibrary.h"
#include "Engine/LevelStreamingDynamic.h"
#include "NavigationSystem.h"
#include "WPPathWall.h"
#include "WPPath.h"
#include "WPPillar.h"
#include "enemyUnit/WPPillarArcher.h"
#include "Kismet/GameplayStatics.h"
#include "enemyUnit/WPBaseEnemyUnit.h"
#include <saveGame/WPPlayerSaveGame.h>
#include <WPGameInstance.h>
#include "player/WPPlayerActor.h"

// LevelBounds
#include "Engine/LevelBounds.h"

// Sets default values
AWPArenaMapGenerator::AWPArenaMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LoadedLevelsCount = 0;

}

// Called when the game starts or when spawned
void AWPArenaMapGenerator::BeginPlay()
{
	Super::BeginPlay();

	UWPPlayerSaveGame* PlayerSaveGame = Cast<UWPPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(Cast<UWPGameInstance>(GetGameInstance())->PlayerSlot, 0));
	Floor = PlayerSaveGame->CurrentFloor;

	ArenaPlacementInit();
	GenerateArenas();

	ConnectArenasMST();
	GeneratePaths();
}

// Called every frame
void AWPArenaMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bDidCallGeneratePath)
	{
		Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn())->GeneratePathForMap(PathPositions);
		bDidCallGeneratePath = 1;
	}

}

// Path를 통해 랜덤한 레벨의 이름을 가져오는 함수
FString AWPArenaMapGenerator::GetRandomLevelName(FString Path) {

	auto ObjectLibrary = UObjectLibrary::CreateLibrary(UWorld::StaticClass(), false, true);
	ObjectLibrary->LoadAssetDataFromPath(Path);
	TArray<FAssetData> AssetDatas;
	ObjectLibrary->GetAssetDataList(AssetDatas);

	TArray<FString> Names = TArray<FString>();

	// 랜덤으로 불러온 레벨들 중 하나를 불러온다.
	int32 RandRange = FMath::RandRange(0, AssetDatas.Num() - 1);
	FAssetData& AssetData = AssetDatas[RandRange];
	auto name = AssetData.AssetName.ToString();

	return name;
}

void AWPArenaMapGenerator::ArenaPlacementInit() {
	// 2차원 배열인 ArenaPlacement를 nullptr로 초기화
	ArenaPlacement.Init(TArray<UWPArena*>(), 7);
	for (auto& Row : ArenaPlacement) {
		Row.Init(nullptr, 7);
	}

	for (int i = 0; i <= 6; i++) { // 5X5 그리드 가쪽에 더미 방을 생성한다.
		for (int j = 0; j <= 6; j++) {
			if (i == 0 || i == 6 || j == 0 || j == 6) {
				ArenaPlacement[i][j] = NewObject<UWPArena>();
				ArenaPlacement[i][j]->Init(
					-1,
					FString::Printf(TEXT("1Floor_1Arena_Dummy")),
					EArenaType::ArenaDummy,
					FVector2D(i, j)
				);
			}
		}
	}

	// 테스트용 DrawDebugBox
	DrawDebugBox(GetWorld(), FVector(5 * -ArenaDistance, 1 * ArenaDistance, 90), FVector(ArenaDistance / 2, ArenaDistance / 2, ArenaDistance / 2), FColor::Green, true, -1, 0, 10);

}

void AWPArenaMapGenerator::GenerateArenas()
{
	for (int i = 1; i <= 5; i++) {
		for (int j = 1; j <= 5; j++) {

			if (ArenaPlacement[i][j] != nullptr) {
				continue;
			}

			int RandRange;
			// 4칸짜리 아레나가 들어갈 수 있는지 확인하고 가능하다면 1,2,4를 RandRange에 모두 활용
			// 불가능하다면 2칸짜리 아레나가 들어갈 수 있는지 확인하고 가능하다면 1,2를 RandRange에 모두 활용
			// 불가능하다면 1칸짜리 아레나를 넣음
			
			RandRange = FMath::RandRange(0, ProbArena1 + ProbArena2 + ProbArena4);
			
			
			// RandRange에 따라서 아레나 생성
			if (RandRange < ProbArena1) {
				PlaceArena1(i, j);
			}
			else if (RandRange < ProbArena1 + ProbArena2) {
				PlaceArena2(i, j);
			}
			else {
				PlaceArena4(i, j);
			}
		}
	}

	// 레벨 인스턴스로 아레나 생성
	for (int i = 0; i < Arenas.Num(); i++) {

		// /Game/Levels/%dFloor/ 로 이루어진 FString Path를 생성한다.
		FString Path = FString::Printf(TEXT("/Game/Levels/%dFloor/"), Floor);
	
		switch (Arenas[i]->ArenaType) {
			case EArenaType::Arena1:
				Path.Append(TEXT("1Arena/"));
				break;
			case EArenaType::ArenaHorizontal2:
				Path.Append(TEXT("2HorizontalArena/"));
				break;
			case EArenaType::ArenaVertical2:
				Path.Append(TEXT("2VerticalArena/"));
				break;
			case EArenaType::Arena4:
				Path.Append(TEXT("4Arena/"));
				break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("ArenaType Error"));
				return;
		}

		FString LevelName = GetRandomLevelName(Path);
		if (LevelName.IsEmpty()) {
			UE_LOG(LogTemp, Warning, TEXT("LevelName is Empty"));
			continue;
		}

		FVector ArenaPosition = FVector(Arenas[i]->ArenaPosition.X * -ArenaDistance, Arenas[i]->ArenaPosition.Y * ArenaDistance, 90);
		 
		bool bLoaded;
		ULevelStreamingDynamic* LevelInstance = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), LevelName, ArenaPosition, FRotator(0,0,0), bLoaded);

		if (LevelInstance) {
			LevelInstance->OnLevelShown.AddDynamic(this, &AWPArenaMapGenerator::OnLevelShown);
			Arenas[i]->LevelInstance = LevelInstance;	

		}

	}

	// 보스방을 생성한다.
	// / Game / Levels / % dFloor / BossArena / 로 이루어진 FString Path를 생성한다.
	FString Path = FString::Printf(TEXT("/Game/Levels/%dFloor/BossArena/"), Floor);
	FString LevelName = GetRandomLevelName(Path);

	// 지도를 만들때 BossName을 Switch문으로 사용하기 위해 저장한다.
	BossName = LevelName.RightChop(LevelName.Find(TEXT("_")) + 1);

	if (LevelName.IsEmpty()) {
		UE_LOG(LogTemp, Warning, TEXT("LevelName is Empty"));
		return;
	}

	FVector BossRoomPosition = FVector(0, 4100, 90);

	bool bLoaded;
	ULevelStreamingDynamic* LevelInstance = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), LevelName, BossRoomPosition, FRotator(0, 0, 0), bLoaded);

	if (LevelInstance) {
		LevelInstance->OnLevelShown.AddDynamic(this, &AWPArenaMapGenerator::OnLevelShown);
	}
	
}

void AWPArenaMapGenerator::OnLevelShown() {
	LoadedLevelsCount++;

	// +1은 보스방을 불러오기 위함
	if (LoadedLevelsCount == Arenas.Num() + 1) {
		OnAllLevelsShown();
	}
}

void AWPArenaMapGenerator::OnAllLevelsShown() {


	TArray<AActor*> Pillars;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWPPillar::StaticClass(), Pillars);

	TArray<AActor*> PillarActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWPPillarArcher::StaticClass(), PillarActors);

	for (AActor* Pillar : Pillars) {
		AWPPillar* PillarActor = Cast<AWPPillar>(Pillar);
		if (PillarActor) {
			for (AActor* PillarArcher : PillarActors) {
				AWPPillarArcher* PillarArcherActor = Cast<AWPPillarArcher>(PillarArcher);
				if (PillarArcherActor) {
					if (PillarActor->GetLevel() == PillarArcherActor->GetLevel()) {
						PillarArcherActor->Pillars.Add(PillarActor);
					}
				}
			}
		}
	}

	if (Fuc_OnAllLevelIsShown.IsBound()) {
		Fuc_OnAllLevelIsShown.Broadcast();
	}


	for (UWPArena* Arena : Arenas)
	{
		for (AActor* ArenaActor : Arena->LevelInstance->GetLoadedLevel()->Actors)
		{
			AWPBaseEnemyUnit* EnemyUnit = Cast<AWPBaseEnemyUnit>(ArenaActor);
			if (EnemyUnit)
			{
				if (Arena->NumOfIcons.Find(EnemyUnit->IconIndex))
					Arena->NumOfIcons[EnemyUnit->IconIndex]++;
				else
					Arena->NumOfIcons.Add(EnemyUnit->IconIndex, 1);
			}
		}
	}
	Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn())->GenerateRoomForMap(Arenas);
	Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetLoadingScreenToFalse();
	Cast<AWPPlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetBossIcon(FCString::Atoi(*BossName));
}

void AWPArenaMapGenerator::PlaceArena4(int i, int j) {
	// PlaceArena 4
	if (ArenaPlacement[i][j + 1] == nullptr && ArenaPlacement[i + 1][j] == nullptr && ArenaPlacement[i + 1][j + 1] == nullptr) {
		UWPArena* Arena4 = NewObject<UWPArena>();
		Arena4->Init(
			Arenas.Num(),
			FString::Printf(TEXT("1Floor_4Arena_Default")),
			EArenaType::Arena4,
			FVector2D(i, j)
		);
		ArenaPlacement[i][j] = Arena4;
		ArenaPlacement[i + 1][j] = Arena4;
		ArenaPlacement[i][j + 1] = Arena4;
		ArenaPlacement[i + 1][j + 1] = Arena4;
		Arenas.Add(Arena4);
		NumArena4++;
	}
	else {
		PlaceArena2(i,j);
	}
}

void AWPArenaMapGenerator::PlaceArena2(int i, int j) {

	if (ArenaPlacement[i + 1][j] != nullptr && ArenaPlacement[i][j + 1] != nullptr) {
		PlaceArena1(i,j);
	}
	else if (ArenaPlacement[i + 1][j] == nullptr && ArenaPlacement[i][j + 1] == nullptr) {
		// 50% 확률로 가로 2칸짜리 아레나 생성, 50% 확률로 세로 2칸짜리 아레나 생성
		int RandRange = FMath::RandRange(0, 1);
		if (RandRange == 0) {
			// 가로 2칸짜리 아레나 생성
			UWPArena* Arena2 = NewObject<UWPArena>();
			Arena2->Init(
				Arenas.Num(),
				FString::Printf(TEXT("1Floor_2HorizontalArena_Default")),
				EArenaType::ArenaHorizontal2,
				FVector2D(i, j)
			);
			ArenaPlacement[i][j] = Arena2;
			ArenaPlacement[i][j + 1] = Arena2;
			Arenas.Add(Arena2);
			NumArena2++;
		}
		else {
			// 세로 2칸짜리 아레나 생성
			UWPArena* Arena2 = NewObject<UWPArena>();
			Arena2->Init(
				Arenas.Num(),
				FString::Printf(TEXT("1Floor_2VerticalArena_Default")),
				EArenaType::ArenaVertical2,
				FVector2D(i, j)
			);
			ArenaPlacement[i][j] = Arena2;
			ArenaPlacement[i + 1][j] = Arena2;
			Arenas.Add(Arena2);
			NumArena2++;
		}
	}
	else if (ArenaPlacement[i + 1][j] != nullptr) {
		// 가로 2칸짜리 아레나 생성
		UWPArena* Arena2 = NewObject<UWPArena>();

		Arena2->Init(
			Arenas.Num(),
			FString::Printf(TEXT("1Floor_2HorizontalArena_Default")),
			EArenaType::ArenaHorizontal2,
			FVector2D(i, j)
		);

		ArenaPlacement[i][j] = Arena2;
		ArenaPlacement[i][j+1] = Arena2;
		Arenas.Add(Arena2);
		NumArena2++;
	}
	else if (ArenaPlacement[i][j + 1] != nullptr) {
		// 세로 2칸짜리 아레나 생성
		UWPArena* Arena2 = NewObject<UWPArena>();
		Arena2->Init(
			Arenas.Num(),
			FString::Printf(TEXT("1Floor_2VerticalArena_Default")),
			EArenaType::ArenaVertical2,
			FVector2D(i, j)
		);
		ArenaPlacement[i][j] = Arena2;
		ArenaPlacement[i+1][j] = Arena2;
		Arenas.Add(Arena2);
		NumArena2++;
	}
	

}

void AWPArenaMapGenerator::PlaceArena1(int i, int j) {

	// PlaceArena 1
	UWPArena* Arena1 = NewObject<UWPArena>();
	Arena1->Init(
		Arenas.Num(),
		FString::Printf(TEXT("1Floor_1Arena_Default")),
		EArenaType::Arena1,
		FVector2D(i, j)
	);
	ArenaPlacement[i][j] = Arena1;
	Arenas.Add(Arena1);
	NumArena1++;

}

void AWPArenaMapGenerator::ConnectArenasMST() {
	TArray<UWPArena*> MSTArenas;
	TArray<UWPArena*> NotMSTArenas;

	NotMSTArenas = Arenas;

	// MST에 들어갈 첫번째 아레나를 랜덤 선택후 MSTArenas로 이동
	int NotMSTRandRange = FMath::RandRange(0, NotMSTArenas.Num() - 1);
	MSTArenas.Add(NotMSTArenas[NotMSTRandRange]);
	NotMSTArenas.RemoveAt(NotMSTRandRange);

	// NotMSTArenas가 비어있을 때까지 반복
	while (NotMSTArenas.Num() > 0) {

		// 1. MSTArenas에서 랜덤으로 아레나를 선택한다.
		// 2. 선택한 아레나와 인접하며 더미가 아닌 아레나를 AdjacentArenas에 추가한다.
		// 3. MSTArenas에 존재하지 않는 아레나를 AdjacentArenas에서 제거한다.
		int MSTRandRange = FMath::RandRange(0, MSTArenas.Num() - 1);
		TArray<TPair<UWPArena*, FVector2D>> AdjacentArenas;

		for (auto& sideVector : MSTArenas[MSTRandRange]->ArenaSide) {

			// 현재 아레나의 위치
			int x = MSTArenas[MSTRandRange]->ArenaPosition.X;
			int y = MSTArenas[MSTRandRange]->ArenaPosition.Y;

			// floor와 ceil을 사용하는 이유는 인접한 아레나의 정확한 위치를 구하기 위함이다.
			// 여기서 diffX,Y가 뜻하는 바는 현재 아레나에서 인접한 아레나로 가는 벡터이다.
			int diffX = sideVector.X >= 0 ? ceil(sideVector.X) : floor(sideVector.X);
			int diffY = sideVector.Y >= 0 ? ceil(sideVector.Y) : floor(sideVector.Y);

			// 현재 아레나의 위치 + 인접한 아레나로의 벡터를 더해,
			// 인접한 아레나의 정확한 위치를 구한다.
			if (ArenaPlacement[x + diffX][y + diffY]->ArenaType != EArenaType::ArenaDummy) {
				AdjacentArenas.AddUnique(TPair<UWPArena*, FVector2D>
					(
						ArenaPlacement[x + diffX][y + diffY], FVector2D(sideVector.X, sideVector.Y)
					)
				);
			}
		}

		// MSTArenas에 존재하지 않는 아레나를 AdjacentArenas에서 제거한다.
		for (int i = 0; i < AdjacentArenas.Num(); i++) {
			if (MSTArenas.Contains(AdjacentArenas[i].Get<0>())) {
				AdjacentArenas.RemoveAt(i);
				i--;
			}
		}

		// AdjacentArenas에 아무것도 없다면 NotMSTArenas에서 제거하고 continue;
		//
		// 그렇지 않다면 AdjacentArenas에서 랜덤으로 아레나를 선택한다.
		// 선택한 인접 아레나를 MSTArenas에 추가하고 NotMSTArenas에서 제거한다.
		// 선택한 인접 아레나와 MSTArenas에서 선택한 아레나 사이에 통로를 뚫는다.
		// 뚫은 통로의 위치를 PathPositions에 추가한다.
		if (AdjacentArenas.Num() == 0) {
			NotMSTArenas.Remove(MSTArenas[MSTRandRange]);
			MSTArenas.RemoveAt(MSTRandRange);
			continue;
		}
		else {
			int AdjacentRandRange = FMath::RandRange(0, AdjacentArenas.Num() - 1);
			MSTArenas.Add(AdjacentArenas[AdjacentRandRange].Get<0>());
			NotMSTArenas.Remove(AdjacentArenas[AdjacentRandRange].Get<0>());

			// 현재 아레나에서 통로를 뚫는다.
			MSTArenas[MSTRandRange]->ConnectedArenas.AddUnique(
				FVector2D(
					AdjacentArenas[AdjacentRandRange].Get<1>().X,
					AdjacentArenas[AdjacentRandRange].Get<1>().Y
				)
			);

			// 현재 아레나에서 뚫을 벽면의 위치
			double sideX = MSTArenas[MSTRandRange]->ArenaPosition.X + AdjacentArenas[AdjacentRandRange].Get<1>().X;
			double sideY = MSTArenas[MSTRandRange]->ArenaPosition.Y + AdjacentArenas[AdjacentRandRange].Get<1>().Y;

			// 반대편 아레나에서 뚫을 벽면의 위치
			double connectedX = sideX - AdjacentArenas[AdjacentRandRange].Get<0>()->ArenaPosition.X;
			double connectedY = sideY - AdjacentArenas[AdjacentRandRange].Get<0>()->ArenaPosition.Y;

			// 반대편도 똑같이 뚫어준다.
			AdjacentArenas[AdjacentRandRange].Get<0>()->ConnectedArenas.AddUnique(
				FVector2D(connectedX, connectedY)
			);

			// PathPositions에 통로의 위치를 저장한다.
			PathPositions.AddUnique(FVector2D(sideX, sideY));
		}
	}


}

void AWPArenaMapGenerator::ConnectAdditionalPaths() {

}

void AWPArenaMapGenerator::GeneratePaths() {

	// 통로 실제 생성
	for (int i = 0; i < PathPositions.Num(); i++) {
		// 만약 PathPositions[i]의 x좌표의 소수점이 .5라면 가로 통로를 생성한다.
		double Rotate;

		if (PathPositions[i].X - floor(PathPositions[i].X) == 0.5) {
			Rotate = 0;
		}
		else {
			Rotate = 90;
		}

		AWPPath* SpawnPath = GetWorld()->SpawnActor<AWPPath>(PathClass, FVector(PathPositions[i].X * -ArenaDistance, PathPositions[i].Y * ArenaDistance, 90), FRotator(0, Rotate, 0));
		SpawnPath->PathIndex = PathPositions[i];
	}
}