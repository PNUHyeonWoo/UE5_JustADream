// Fill out your copyright notice in the Description page of Project Settings.


#include "generateMap/WPArena.h"

void UWPArena::Init(int32 FArenaID, FString FArenaName, EArenaType FArenaType, FVector2D FArenaPosition)
{
	this->ArenaID = FArenaID;
	this->ArenaName = FArenaName;
	this->ArenaType = FArenaType;
	this->ArenaPosition = FArenaPosition;
	this->ConnectedArenas = TArray<FVector2D>();

	if (ArenaType == EArenaType::Arena1) {
		this->ArenaSide.Add(FVector2D(-0.5, 0));
		this->ArenaSide.Add(FVector2D(0, 0.5));
		this->ArenaSide.Add(FVector2D(0.5, 0));
		this->ArenaSide.Add(FVector2D(0, -0.5));
	}
	else if (ArenaType == EArenaType::ArenaHorizontal2) {
		this->ArenaSide.Add(FVector2D(-0.5, 0));
		this->ArenaSide.Add(FVector2D(-0.5, 1));
		this->ArenaSide.Add(FVector2D(0, 1.5));
		this->ArenaSide.Add(FVector2D(0.5, 1));
		this->ArenaSide.Add(FVector2D(0.5, 0));
		this->ArenaSide.Add(FVector2D(0, -0.5));
	}
	else if (ArenaType == EArenaType::ArenaVertical2) {
		this->ArenaSide.Add(FVector2D(-0.5, 0));
		this->ArenaSide.Add(FVector2D(0, 0.5));
		this->ArenaSide.Add(FVector2D(1, 0.5));
		this->ArenaSide.Add(FVector2D(1.5, 0));
		this->ArenaSide.Add(FVector2D(1, -0.5));
		this->ArenaSide.Add(FVector2D(0, -0.5));

	}
	else if (ArenaType == EArenaType::Arena4) {
		this->ArenaSide.Add(FVector2D(-0.5, 0));
		this->ArenaSide.Add(FVector2D(-0.5, 1));
		this->ArenaSide.Add(FVector2D(0, 1.5));
		this->ArenaSide.Add(FVector2D(1, 1.5));
		this->ArenaSide.Add(FVector2D(1.5, 1));
		this->ArenaSide.Add(FVector2D(1.5, 0));
		this->ArenaSide.Add(FVector2D(1, -0.5));
		this->ArenaSide.Add(FVector2D(0, -0.5));
	}

}