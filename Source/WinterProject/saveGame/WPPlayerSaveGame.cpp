// Fill out your copyright notice in the Description page of Project Settings.


#include "saveGame/WPPlayerSaveGame.h"
#include <dataAsset/WPGunDataAsset.h>
#include <dataAsset/WPAmmoDataAsset.h>
#include <Kismet/GameplayStatics.h>
#include <Engine/AssetManager.h>
#include "WPGameInstance.h"

UWPPlayerSaveGame* UWPPlayerSaveGame::CreateNewPlayerSaveGame()
{
	const FName StartGun = TEXT("revolver");

	const int8 ItemCapacity = 5;


	UWPPlayerSaveGame* PlayerSaveGame = Cast<UWPPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UWPPlayerSaveGame::StaticClass()));

	PlayerSaveGame->CurrentFloor = 1;

	PlayerSaveGame->Gold = 0;

	UAssetManager& Manager = UAssetManager::Get();

	TArray<FAssetData> GunAssets;
	Manager.GetPrimaryAssetDataList(TEXT("WPGunDataAsset"), GunAssets);

	TMap<FName, uint8> GunPurchase;

	for (FAssetData AssetData : GunAssets)
		GunPurchase.Add(AssetData.AssetName, 0);

	GunPurchase[StartGun] = 1;

	PlayerSaveGame->GunPurchase = GunPurchase;

	PlayerSaveGame->GearLevel = 0;

	PlayerSaveGame->PossessedGuns.Add(TEXT(""));
	PlayerSaveGame->PossessedGuns.Add(StartGun);

	PlayerSaveGame->EquippedGun = 1;

	
	UWPGunDataAsset* StartGunAsset = UWPGunDataAsset::GetGunData(StartGun);
	UWPAmmoDataAsset* StartGunAmmo = StartGunAsset->AmmoUsed;

	PlayerSaveGame->NumOfAmmoInChamber.Add(0);
	PlayerSaveGame->NumOfAmmoInChamber.Add(StartGunAsset->ChamberCapacity);

	PlayerSaveGame->NumOfAmmoInMagazine.Add(0);
	PlayerSaveGame->NumOfAmmoInMagazine.Add(StartGunAsset->MagazineCapacity);

	PlayerSaveGame->AmmoInGearUpper = TArray<int8>();

	PlayerSaveGame->FillAmmoInGear(1);

	TArray<TObjectPtr<UWPItemDataAsset>> ItemInGear;
	ItemInGear.Init(nullptr, ItemCapacity);

	PlayerSaveGame->ItemsInGear = ItemInGear;

	TArray<float> ItemAmounts;
	ItemAmounts.Init(0, ItemCapacity);

	PlayerSaveGame->ItemAmounts = ItemAmounts;

	PlayerSaveGame->MaxHP = 100;

	PlayerSaveGame->CurrentHP = PlayerSaveGame->MaxHP;

	PlayerSaveGame->MaxStamina = 100;

	PlayerSaveGame->CurrentStamina = PlayerSaveGame->MaxStamina;

	TMap<FName, float> StatusEffect;

	TArray<FAssetData> StatusEffectAssets;
	Manager.GetPrimaryAssetDataList(TEXT("WPStatusEffectDataAsset"), StatusEffectAssets);

	for (FAssetData AssetData : StatusEffectAssets)
		StatusEffect.Add(AssetData.AssetName, 0);

	PlayerSaveGame->StatusEffect = StatusEffect;

	PlayerSaveGame->StatusEffectResistance = StatusEffect;

	PlayerSaveGame->StaminaRecovery = 10;

	PlayerSaveGame->StaminaConsumption = 10;

	PlayerSaveGame->WalkMaxSpeed = 300;
	PlayerSaveGame->DefaultWalkMaxSpeed = PlayerSaveGame->WalkMaxSpeed;

	PlayerSaveGame->RunAcceleration = 1400;
	PlayerSaveGame->DefaultRunAcceleration = PlayerSaveGame->RunAcceleration;

	PlayerSaveGame->RunMaxSpeed = 700;
	PlayerSaveGame->DefaultRunMaxSpeed = PlayerSaveGame->RunMaxSpeed;

	PlayerSaveGame->AimWalkMaxSpeed = 150;
	PlayerSaveGame->DefaultAimWalkMaxSpeed = PlayerSaveGame->AimWalkMaxSpeed;

	PlayerSaveGame->RecoilControl = 0;

	PlayerSaveGame->ReloadSpeed = 1;

	PlayerSaveGame->DiscountRate = 0;

	PlayerSaveGame->DropRate = 100;

	PlayerSaveGame->CumulativeEXP = 0;

	
	if (1) // if skill enable
	{
		UWPGameInstance* WPGameInstance = Cast<UWPGameInstance>(UGameplayStatics::GetGameInstance(GEngine->GameViewport->GetWorld()));

		const float MaxHPInc = 15;
		const float MaxStaminaInc = 15;
		const float SpeedInc = 0.1;
		const uint32 GoldInc = 100;
		const float ResistanceInc = 0.2;
		const float RecoilControlInc = 0.1;

		PlayerSaveGame->MaxHP += WPGameInstance->GetSkillLevel(TEXT("MaxHP")) * MaxHPInc;
		PlayerSaveGame->CurrentHP = PlayerSaveGame->MaxHP;

		PlayerSaveGame->MaxStamina += WPGameInstance->GetSkillLevel(TEXT("MaxStamina")) * MaxStaminaInc;
		PlayerSaveGame->CurrentStamina = PlayerSaveGame->MaxStamina;

		float SpeedIncRatio = 1 + WPGameInstance->GetSkillLevel(TEXT("Speed")) * SpeedInc;
		PlayerSaveGame->WalkMaxSpeed *= SpeedIncRatio;
		PlayerSaveGame->RunMaxSpeed *= SpeedIncRatio;
		PlayerSaveGame->RunAcceleration *= SpeedIncRatio;
		PlayerSaveGame->AimWalkMaxSpeed *= SpeedIncRatio;

		PlayerSaveGame->DefaultWalkMaxSpeed = PlayerSaveGame->WalkMaxSpeed;
		PlayerSaveGame->DefaultRunAcceleration = PlayerSaveGame->RunAcceleration;
		PlayerSaveGame->DefaultRunMaxSpeed = PlayerSaveGame->RunMaxSpeed;
		PlayerSaveGame->DefaultAimWalkMaxSpeed = PlayerSaveGame->AimWalkMaxSpeed;


		PlayerSaveGame->Gold += WPGameInstance->GetSkillLevel(TEXT("StartingGold")) * GoldInc;

		for (TPair<FName, float> &ResistancePair: PlayerSaveGame->StatusEffectResistance)
		{
			ResistancePair.Value += WPGameInstance->GetSkillLevel(TEXT("Resistance")) * ResistanceInc;
		}

		PlayerSaveGame->RecoilControl += WPGameInstance->GetSkillLevel(TEXT("RecoilControl")) * RecoilControlInc;

	}
	


	return PlayerSaveGame;
}

void UWPPlayerSaveGame::FillAmmoInGun(uint8 GunType)
{
	FName GunName = PossessedGuns[GunType];
	if (GunName == TEXT(""))
		return;

	UWPGunDataAsset* GunData = UWPGunDataAsset::GetGunData(GunName);

	NumOfAmmoInChamber[GunType] = GunData->ChamberCapacity;
	NumOfAmmoInMagazine[GunType] = GunData->MagazineCapacity;
}

void UWPPlayerSaveGame::FillAmmoInGear(uint8 GunType)
{
	FName GunName = PossessedGuns[GunType];
	if (GunName == TEXT(""))
		return;

	UWPGunDataAsset* GunData = UWPGunDataAsset::GetGunData(GunName);
	UWPAmmoDataAsset* AmmoData = GunData->AmmoUsed;

	int8 AmmoElement = 0;
	if (AmmoData->isMagazine)
		AmmoElement = GunData->MagazineCapacity;

	TArray<int8> Ammos;
	Ammos.Init(AmmoElement, GunData->NumOfAmmoInGear[GearLevel]);

	if (GunType == 0)
		AmmoInGearUpper = Ammos;
	else
		AmmoInGearLower = Ammos;

}

UWPGunDataAsset* UWPPlayerSaveGame::GetEquippedGunData()
{
	return UWPGunDataAsset::GetGunData(PossessedGuns[EquippedGun]);
}
