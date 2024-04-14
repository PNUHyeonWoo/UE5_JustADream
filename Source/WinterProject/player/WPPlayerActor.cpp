// Fill out your copyright notice in the Description page of Project Settings.


#include "player/WPPlayerActor.h"
#include "Animation/AnimNode_StateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "saveGame/WPPlayerSaveGame.h"
#include "WPGameInstance.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
#include "player/WPPlayerAnimInstance.h"
#include <dataAsset/WPGunDataAsset.h>
#include <dataAsset/WPAmmoDataAsset.h>
#include "userWidget/player/WPPlayerHUIUserWidget.h"
#include "player/WPGunAnimInstance.h"
#include "dataAsset/item/WPItemDataAsset.h"
#include "interaction/WPDropItem.h"
#include "NiagaraFunctionLibrary.h"
#include <interface/WPInteractionInterface.h>
#include "dataAsset/statusEffect/WPStatusEffectDataAsset.h"
#include "Perception/AISense_Hearing.h"
#include "NiagaraComponent.h"

// Sets default values
AWPPlayerActor::AWPPlayerActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CanRecoverStamina = 1;
	CanRun = 1;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	MainGunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MainGunMesh"));
	MainGunMeshComponent->SetupAttachment(GetMesh(),TEXT("StomachSocket"));

	SubGunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SubGunMesh"));
	SubGunMeshComponent->SetupAttachment(GetMesh(), TEXT("RHandSocket"));

}

// Called when the game starts or when spawned
void AWPPlayerActor::BeginPlay()
{
	Super::BeginPlay();
	PlayerSaveGame = Cast<UWPPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(Cast<UWPGameInstance>(GetGameInstance())->PlayerSlot, 0));
	SetMaxHP(PlayerSaveGame->MaxHP);
	SetCurrentHP(PlayerSaveGame->CurrentHP);

	initGun();

	//Å×½ºÆ®¿ë
	//PlayerSaveGame->Gold = 1000;
	//ReceiveGun(TEXT("doubleBarrelShotgun"));
	//ReceiveGun(TEXT("pistol"));
	//AWPDropItem::CreateDropItem(GetWorld(), FVector(0, 0, 300), TEXT("instantHealingPotion1"));
	//ReceiveGold(1000);

	//PlayerSaveGame->StatusEffect[TEXT("bleeding")] = 10;
	//PlayerSaveGame->StatusEffect[TEXT("poison")] = 10;

	//TryToReceiveItem(TEXT("speedBuffPortion1"));


	//ReceiveGun(TEXT("mosinagant"));
	//ReceiveGun(TEXT("pumpshotgun"));
	//SwapGun();

	//UGameplayStatics::SaveGameToSlot(PlayerSaveGame, Cast<UWPGameInstance>(GetGameInstance())->PlayerSlot, 0);
}

// Called every frame
void AWPPlayerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead)
		return;

	ShoulderHeight = GetActorLocation().Z + ShoulderRelativeHeight;

	FName NowState = GetMesh()->GetAnimInstance()->GetStateMachineInstanceFromName(TEXT("UpperStateMachine"))->GetCurrentStateName();
	RotatePlayer(NowState);
	MovePlayer(NowState);
	TickStamina(DeltaTime);
	TickStatusEffect(DeltaTime);

}

// Called to bind functionality to input
void AWPPlayerActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveY", this, &AWPPlayerActor::InputMoveY);
	InputComponent->BindAxis("MoveX", this, &AWPPlayerActor::InputMoveX);
	InputComponent->BindAction("Run", IE_Pressed, this, &AWPPlayerActor::PressRun);
	InputComponent->BindAction("Run", IE_Released, this, &AWPPlayerActor::ReleaseRun);
	InputComponent->BindAction("Bolt", IE_Pressed, this, &AWPPlayerActor::PressBolt);
	InputComponent->BindAction("Bolt", IE_Released, this, &AWPPlayerActor::ReleaseBolt);
	InputComponent->BindAction("Swap", IE_Pressed, this, &AWPPlayerActor::PressSwap);
	InputComponent->BindAction("Swap", IE_Released, this, &AWPPlayerActor::ReleaseSwap);
	InputComponent->BindAction("Shot", IE_Pressed, this, &AWPPlayerActor::PressShot);
	InputComponent->BindAction("Aim", IE_Pressed, this, &AWPPlayerActor::PressAim);
	InputComponent->BindAction("Interaction", IE_Pressed, this, &AWPPlayerActor::PressInteraction);
	InputComponent->BindAction("Map", IE_Pressed, this, &AWPPlayerActor::PressMap);
	InputComponent->BindAction("Menu", IE_Pressed, this, &AWPPlayerActor::PressMenu);
}

void AWPPlayerActor::WPTakeDamage(float Damage, float Penetration, uint8 HitsWeakness, TArray<FName> StatusEffectNames, TArray<float> StatusEffectAmount, FVector HitDirection, FVector HitPosition)
{
	IWPAttackTargetInterface::WPTakeDamage(Damage, Penetration, HitsWeakness, StatusEffectNames, StatusEffectAmount, HitDirection, HitPosition);
	if (!IsDead)
	{
		for (int i = 0; i < StatusEffectNames.Num(); i++)
			PlayerSaveGame->StatusEffect[StatusEffectNames[i]] += StatusEffectAmount[i] * (1 - PlayerSaveGame->StatusEffectResistance[StatusEffectNames[i]]);

		if (Damage > 10)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, GetActorLocation());
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		}

	}
}

void AWPPlayerActor::Die()
{
	PlayerHUIUserWidget->HideShop();

	Cast<UWPPlayerAnimInstance>(GetMesh()->GetAnimInstance())->IsDead = 1;
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	SetPlayerInput(false);
	GetCharacterMovement()->Velocity = { 0,0,0 };

	GetWorldTimerManager().SetTimer(DeadWidgetHandle, this, &AWPPlayerActor::OpenPlayerDeadWidget, 3, false);

	UGameplayStatics::PlaySoundAtLocation(this, DieSound, GetActorLocation());
}

void AWPPlayerActor::initGun()
{
	ReceiveGun(PlayerSaveGame->PossessedGuns[0]);
	ReceiveGun(PlayerSaveGame->PossessedGuns[1]);
	PlayerHUIUserWidget->RefreshGear(2);
	SetGunPosition();
}

void AWPPlayerActor::SetGunPosition()
{
	if (PlayerSaveGame->EquippedGun == 0)
	{
		MainGunMeshComponent->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform, TEXT("RHandSocket"));
		SubGunMeshComponent->SetVisibility(false);
	}
	else 
	{
		MainGunMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("StomachSocket"));
		SubGunMeshComponent->SetVisibility(true);
	}

	ShoulderLength = ShoulderLengths[PlayerSaveGame->EquippedGun];
}

void AWPPlayerActor::RotatePlayer(FName NowState)
{
	if (IsRunning)
		SetIsTrueAiming(0);

	if (GetVelocity().Length() < GetBasedOnRunningSpeed())
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		AimWithShoulder();
	}
	else 
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	
}

void AWPPlayerActor::AimWithShoulder()
{
	FVector AimTargetPosition = GetMouseAimPosition(ShoulderHeight);

	FVector2D PlayerToTarget = { AimTargetPosition.X - GetActorLocation().X,AimTargetPosition.Y - GetActorLocation().Y };


	float Length = PlayerToTarget.Length();

	if (Length < abs(ShoulderLength))
	{
		Length = abs(ShoulderLength);
		PlayerToTarget = PlayerToTarget / PlayerToTarget.Length() * Length;
	}

	float Angle;

	if (PlayerToTarget.Y <= 0)
		Angle = (-FMath::Asin(ShoulderLength / Length) - FMath::Acos(PlayerToTarget.X / Length)) * 180 / PI;
	else
		Angle = (-FMath::Asin(ShoulderLength / Length) + FMath::Acos(PlayerToTarget.X / Length)) * 180 / PI;

	SetActorRotation(FRotator(0, Angle, 0));

}

void AWPPlayerActor::MovePlayer(FName NowState)
{
	if (IsRunning && PlayerSaveGame->CurrentStamina > 0)
	{
		if (NowState == TEXT("Aiming") || NowState == TEXT("Running"))
			GetCharacterMovement()->MaxWalkSpeed = PlayerSaveGame->RunMaxSpeed;
		else
			GetCharacterMovement()->MaxWalkSpeed = (PlayerSaveGame->RunMaxSpeed + PlayerSaveGame->WalkMaxSpeed)/2;	
	}
	else 
	{
		if(IsTrueAiming)
			GetCharacterMovement()->MaxWalkSpeed = PlayerSaveGame->AimWalkMaxSpeed;
		else
			GetCharacterMovement()->MaxWalkSpeed = PlayerSaveGame->WalkMaxSpeed;
	}

	if (!InputEnabled())
		MoveDirection = { 0,0 };

	MoveDirection.Normalize();
	AddMovementInput(FVector(MoveDirection.X, MoveDirection.Y,0), 1.0f);
}

void AWPPlayerActor::TickStamina(float DeltaTime)
{
	if (PlayerSaveGame->CurrentStamina > 20)
	{
		CanRun = 1;
		if (PlayerHUIUserWidget)
			PlayerHUIUserWidget->SetStaminaBarCanRun(true);
	}

	if (IsRunning && GetVelocity().Length() >= GetBasedOnRunningSpeed() && PlayerSaveGame->CurrentStamina > 0 && CanRecoverStamina)
	{
		PlayerSaveGame->CurrentStamina -= PlayerSaveGame->StaminaConsumption * DeltaTime;
		if (PlayerSaveGame->CurrentStamina <= 0)
		{
			PlayerSaveGame->CurrentStamina = 0;
			CanRecoverStamina = 0;
			GetWorldTimerManager().SetTimer(StaminaRecoveryHandle, this, &AWPPlayerActor::SetCanRecoverStaminaTrue, 0.1, false);
			IsRunning = 0;
			CanRun = 0;
			if (PlayerHUIUserWidget)
				PlayerHUIUserWidget->SetStaminaBarCanRun(false);

		}
	}
	else if (CanRecoverStamina)
	{
		PlayerSaveGame->CurrentStamina += PlayerSaveGame->StaminaRecovery * DeltaTime;
		if (PlayerSaveGame->MaxStamina < PlayerSaveGame->CurrentStamina)
			PlayerSaveGame->CurrentStamina = PlayerSaveGame->MaxStamina;
	}
}

void AWPPlayerActor::TickStatusEffect(float DeltaTime)
{
	if (PlayerHUIUserWidget && !bInitStatusEffectImages)
	{
		PlayerHUIUserWidget->CreateStatusEffectImages(PlayerSaveGame->StatusEffect);
		bInitStatusEffectImages = true;
	}

	for (TTuple<FName,float> StatusEffect : PlayerSaveGame->StatusEffect)
	{
		float NewAmount = UWPStatusEffectDataAsset::GetStatusEffectData(StatusEffect.Key)->TickStatusEffect(this, StatusEffect.Value, PlayerSaveGame->StatusEffectResistance[StatusEffect.Key], DeltaTime);
		PlayerSaveGame->StatusEffect[StatusEffect.Key] = NewAmount < 0 ? 0 : NewAmount;
		if (bInitStatusEffectImages)
			PlayerHUIUserWidget->SetStatusEffectImageVisiblity(StatusEffect.Key, NewAmount > 0);
	}
}

float AWPPlayerActor::GetBasedOnRunningSpeed()
{
	if (!PlayerSaveGame)
		return 0;

	return (PlayerSaveGame->RunMaxSpeed + 3 * PlayerSaveGame->WalkMaxSpeed) / 4;
}

void AWPPlayerActor::SetPlayerHUIUserWidget(UWPPlayerHUIUserWidget* newWidget)
{
	PlayerHUIUserWidget = newWidget;
}

uint8 AWPPlayerActor::GetIsTrueAiming()
{
	return IsTrueAiming;
}

void AWPPlayerActor::ReceiveGun(FName GunName)
{
	if (GunName != TEXT(""))
	{
		UWPGunDataAsset* GunData = UWPGunDataAsset::GetGunData(GunName);

		//¼ÒÁö ÃÑ±â °»½Å
		PlayerSaveGame->PossessedGuns[GunData->GunType] = GunName;

		//¾à½Ç, ÅºÃ¢, ±â¾î ÃÑ¾Ë, ui °»½Å
		FillAmmo(GunData->GunType);

		//¸öÃ¼ animInstance ÀåÂø ÃÑ±â °»½Å
		if(PlayerSaveGame->EquippedGun == GunData->GunType)
			Cast<UWPPlayerAnimInstance>(GetMesh()->GetAnimInstance())->AnimationIndex = GunData->AnimationIndex;

		//ÃÑ±â ½ºÄÌ·¹Å»

		USkeletalMeshComponent* GunMeshComponent;
		if (GunData->GunType == 0)
			GunMeshComponent = MainGunMeshComponent;
		else
			GunMeshComponent = SubGunMeshComponent;

		GunMeshComponent->SetSkeletalMesh(GunData->GunMesh);
		GunMeshComponent->SetAnimInstanceClass(GunData->GunAnimationBP.Get());

		if (PlayerSaveGame->EquippedGun == GunData->GunType)
			SetGunAnimInstanceBolt(false);
	}
}

void AWPPlayerActor::FillAmmo(uint8 GunType)
{
	if (PlayerSaveGame->EquippedGun == GunType)
		SetGunAnimInstanceBolt(false);

	PlayerSaveGame->FillAmmoInGun(GunType);
	PlayerSaveGame->FillAmmoInGear(GunType);
	PlayerHUIUserWidget->RefreshGear(GunType);
}

void AWPPlayerActor::SwapGun()
{
	if (PlayerSaveGame->PossessedGuns[1 - PlayerSaveGame->EquippedGun] == TEXT(""))
		return;

	SetGunAnimInstanceBolt(false);

	PlayerSaveGame->EquippedGun = 1 - PlayerSaveGame->EquippedGun;
	Cast<UWPPlayerAnimInstance>(GetMesh()->GetAnimInstance())->AnimationIndex = PlayerSaveGame->GetEquippedGunData()->AnimationIndex;
	SetGunPosition();

}

void AWPPlayerActor::ChamberLoad()
{
	if (PlayerSaveGame->GetEquippedGunData()->MagazineCapacity > 0) 
	{
		uint8 EquippedGunIndex = PlayerSaveGame->EquippedGun;

		if (PlayerSaveGame->NumOfAmmoInChamber[EquippedGunIndex] > 0)
			PlayerSaveGame->NumOfAmmoInChamber[EquippedGunIndex]--;

		if (PlayerSaveGame->NumOfAmmoInMagazine[EquippedGunIndex] > 0)
		{
			PlayerSaveGame->NumOfAmmoInMagazine[EquippedGunIndex]--;
			PlayerSaveGame->NumOfAmmoInChamber[EquippedGunIndex]++;
		}
	}
}

void AWPPlayerActor::ClickItem(uint8 isLeftClick, uint8 YIndex, uint8 XIndex)
{
	if (IsDead)
		return;

	if (YIndex < 2) // clicked ammo
	{
		UWPGunDataAsset* GunData = PlayerSaveGame->GetEquippedGunData();
		UWPAmmoDataAsset* AmmoData = GunData->AmmoUsed;
		if (UWPGunDataAsset::GetGunData(PlayerSaveGame->PossessedGuns[YIndex])->AmmoUsed->GetPrimaryAssetId() != AmmoData->GetPrimaryAssetId())
			return;

		if (YIndex == 0 && PlayerSaveGame->AmmoInGearUpper[XIndex] == -1)
		{
			UE_LOG(LogPlayerManagement, Warning, TEXT("Try to -1 ammo reload"));
			return;
		}

		if (YIndex == 1 && PlayerSaveGame->AmmoInGearLower[XIndex] == -1)
		{
			UE_LOG(LogPlayerManagement, Warning, TEXT("Try to -1 ammo reload"));
			return;
		}

		FName NowState = GetMesh()->GetAnimInstance()->GetStateMachineInstanceFromName(TEXT("UpperStateMachine"))->GetCurrentStateName();

		if (NowState == TEXT("Aiming") || NowState == TEXT("Running"))
		{
			if (!GunData->CanNormalReload)
				return;

			if (AmmoData->isMagazine == 0 && PlayerSaveGame->NumOfAmmoInMagazine[PlayerSaveGame->EquippedGun] >= GunData->MagazineCapacity)
				return;

			AmmoToReload.Key = YIndex;
			AmmoToReload.Value = XIndex;

			if (isLeftClick)
				Cast<UWPPlayerAnimInstance>(GetMesh()->GetAnimInstance())->TryToNormalReload = 1;
			else
			{
				if (GunData->CanFastReload)
					Cast<UWPPlayerAnimInstance>(GetMesh()->GetAnimInstance())->TryToFastReload = 1;
				else
					Cast<UWPPlayerAnimInstance>(GetMesh()->GetAnimInstance())->TryToNormalReload = 1;
			}
		}
		else if (NowState == TEXT("BoltOn"))
		{
			if (!GunData->CanChamberReload)
				return;

			if (GunData->CanReloadMagazineThroughChamber)
			{
				if (PlayerSaveGame->NumOfAmmoInChamber[PlayerSaveGame->EquippedGun] >= GunData->ChamberCapacity && PlayerSaveGame->NumOfAmmoInMagazine[PlayerSaveGame->EquippedGun] >= GunData->MagazineCapacity)
					return;
			}
			else 
			{
				if (PlayerSaveGame->NumOfAmmoInChamber[PlayerSaveGame->EquippedGun] >= GunData->ChamberCapacity)
					return;
			}

			AmmoToReload.Key = YIndex;
			AmmoToReload.Value = XIndex;
			Cast<UWPPlayerAnimInstance>(GetMesh()->GetAnimInstance())->TryToChamberReload = 1;
		}
	}
	else // clicked item
	{
		if (PlayerSaveGame->ItemsInGear[XIndex] == nullptr)
		{
			UE_LOG(LogPlayerManagement, Warning, TEXT("Try to empty item use"));
			return;
		}

		if (isLeftClick)
		{
			PlayerSaveGame->ItemAmounts[XIndex] = PlayerSaveGame->ItemsInGear[XIndex]->UseItem(this, PlayerSaveGame->ItemAmounts[XIndex]);

			if (PlayerSaveGame->ItemAmounts[XIndex] <= 0)
			{
				PlayerSaveGame->ItemsInGear[XIndex] = nullptr;
				PlayerSaveGame->ItemAmounts[XIndex] = 0;
				PlayerHUIUserWidget->RefreshGear(2);
			}
		}
		else 
		{
			if (PlayerSaveGame->ItemsInGear[XIndex] != nullptr)
			{
				AWPDropItem::CreateDropItem(GetWorld(), GetActorLocation() + GetActorForwardVector() * 50, PlayerSaveGame->ItemsInGear[XIndex]->GetPrimaryAssetId().PrimaryAssetName, PlayerSaveGame->ItemAmounts[XIndex]);

				PlayerSaveGame->ItemsInGear[XIndex] = nullptr;
				PlayerSaveGame->ItemAmounts[XIndex] = 0;
				PlayerHUIUserWidget->RefreshGear(2);
			}
		}

	}


}

const UWPPlayerSaveGame* AWPPlayerActor::GetPlayerSaveGame()
{
	return PlayerSaveGame;
}

UWPGunDataAsset* AWPPlayerActor::GetEquippedGunData()
{
	if (!PlayerSaveGame)
		return nullptr;

	return PlayerSaveGame->GetEquippedGunData();
}

void AWPPlayerActor::Reload(uint8 ReloadType)
{
	UWPGunDataAsset* GunData = PlayerSaveGame->GetEquippedGunData();
	UWPAmmoDataAsset* AmmoData = GunData->AmmoUsed;
	if (UWPGunDataAsset::GetGunData(PlayerSaveGame->PossessedGuns[AmmoToReload.Key])->AmmoUsed->GetPrimaryAssetId() != AmmoData->GetPrimaryAssetId())
	{
		UE_LOG(LogTemp, Warning, TEXT("Try to unsuitable ammo true reload"));
		return;
	}

	if (AmmoToReload.Key == 0 && PlayerSaveGame->AmmoInGearUpper[AmmoToReload.Value] == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Try to -1 ammo true reload"));
		return;
	}

	if (AmmoToReload.Key == 1 && PlayerSaveGame->AmmoInGearLower[AmmoToReload.Value] == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Try to -1 ammo true reload"));
		return;
	}

	

	if (ReloadType == 0)
	{
		if (AmmoData->isMagazine == 0) // ammo normal reload
		{
			if (PlayerSaveGame->NumOfAmmoInMagazine[PlayerSaveGame->EquippedGun] >= GunData->MagazineCapacity)
			{
				UE_LOG(LogTemp, Warning, TEXT("Try to max magazine true reload"));
				return;
			}

			if (AmmoToReload.Key == 0)
				PlayerSaveGame->AmmoInGearUpper[AmmoToReload.Value] = -1;
			else if(AmmoToReload.Key == 1)
				PlayerSaveGame->AmmoInGearLower[AmmoToReload.Value] = -1;

			PlayerSaveGame->NumOfAmmoInMagazine[PlayerSaveGame->EquippedGun]++;
		}
		else // magainze normal reload 
		{
			int8 NewMagazineAmount;

			if (AmmoToReload.Key == 0)
			{
				NewMagazineAmount = PlayerSaveGame->AmmoInGearUpper[AmmoToReload.Value];
				PlayerSaveGame->AmmoInGearUpper[AmmoToReload.Value] = PlayerSaveGame->NumOfAmmoInMagazine[PlayerSaveGame->EquippedGun];
			}
			else if (AmmoToReload.Key == 1)
			{
				NewMagazineAmount = PlayerSaveGame->AmmoInGearLower[AmmoToReload.Value];
				PlayerSaveGame->AmmoInGearLower[AmmoToReload.Value] = PlayerSaveGame->NumOfAmmoInMagazine[PlayerSaveGame->EquippedGun];
			}

			PlayerSaveGame->NumOfAmmoInMagazine[PlayerSaveGame->EquippedGun] = NewMagazineAmount;

			if (GunData->CanRackBolt && GetGunAnimInstanceBolt())
			{
				ChamberLoad();
				SetGunAnimInstanceBolt(false);
			}

		}

	}
	else if (ReloadType == 1) // fast reload
	{
		if (AmmoData->isMagazine == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Try to non magazine gun true fast reload"));
			return;
		}

		if (AmmoToReload.Key == 0)
		{
			PlayerSaveGame->NumOfAmmoInMagazine[PlayerSaveGame->EquippedGun] = PlayerSaveGame->AmmoInGearUpper[AmmoToReload.Value];
			PlayerSaveGame->AmmoInGearUpper[AmmoToReload.Value] = -1;
		}
		else if (AmmoToReload.Key == 1)
		{
			PlayerSaveGame->NumOfAmmoInMagazine[PlayerSaveGame->EquippedGun] = PlayerSaveGame->AmmoInGearLower[AmmoToReload.Value];
			PlayerSaveGame->AmmoInGearLower[AmmoToReload.Value] = -1;
		}

		if (GunData->CanRackBolt && GetGunAnimInstanceBolt())
		{
			ChamberLoad();
			SetGunAnimInstanceBolt(false);
		}

	}
	else if (ReloadType == 2) //chmaber reload
	{
		if (AmmoData->isMagazine == 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Try to magazine gun true chamber reload"));
			return;
		}

		if (GunData->CanReloadMagazineThroughChamber)
		{
			if (PlayerSaveGame->NumOfAmmoInChamber[PlayerSaveGame->EquippedGun] >= GunData->ChamberCapacity && PlayerSaveGame->NumOfAmmoInMagazine[PlayerSaveGame->EquippedGun] >= GunData->MagazineCapacity)
			{
				UE_LOG(LogTemp, Warning, TEXT("Try to max chamber and magazine true reload"));
				return;
			}
		}
		else
		{
			if (PlayerSaveGame->NumOfAmmoInChamber[PlayerSaveGame->EquippedGun] >= GunData->ChamberCapacity)
			{
				UE_LOG(LogTemp, Warning, TEXT("Try to max chamber true reload"));
				return;
			}
		}

		if (AmmoToReload.Key == 0)
			PlayerSaveGame->AmmoInGearUpper[AmmoToReload.Value] = -1;
		else if (AmmoToReload.Key == 1)
			PlayerSaveGame->AmmoInGearLower[AmmoToReload.Value] = -1;

		PlayerSaveGame->NumOfAmmoInChamber[PlayerSaveGame->EquippedGun]++;

		if (PlayerSaveGame->NumOfAmmoInChamber[PlayerSaveGame->EquippedGun] > GunData->ChamberCapacity)
		{
			PlayerSaveGame->NumOfAmmoInChamber[PlayerSaveGame->EquippedGun]--;
			PlayerSaveGame->NumOfAmmoInMagazine[PlayerSaveGame->EquippedGun]++;
		}

		PlayerHUIUserWidget->SetRemainAmmoVisible((float)(PlayerSaveGame->NumOfAmmoInChamber[PlayerSaveGame->EquippedGun] + PlayerSaveGame->NumOfAmmoInMagazine[PlayerSaveGame->EquippedGun]) / (GunData->ChamberCapacity + GunData->MagazineCapacity));
	}

	PlayerHUIUserWidget->RefreshGear(AmmoToReload.Key);

}

void AWPPlayerActor::SetGunAnimInstanceBolt(bool Value)
{
	UWPGunAnimInstance* GunAnim;
	if (PlayerSaveGame->EquippedGun == 0)
		GunAnim = Cast<UWPGunAnimInstance>(MainGunMeshComponent->GetAnimInstance());
	else 
		GunAnim = Cast<UWPGunAnimInstance>(SubGunMeshComponent->GetAnimInstance());

	if (GunAnim)
	{
		UWPGunDataAsset* GunData = PlayerSaveGame->GetEquippedGunData();

		if(GunAnim->PullBolt && !Value)
			UGameplayStatics::PlaySoundAtLocation(this, GunData->BoltOffSound, GetActorLocation());
		else if (!GunAnim->PullBolt && Value)
			UGameplayStatics::PlaySoundAtLocation(this, GunData->BoltOnSound, GetActorLocation());


		if (GunData->CanChamberReload && Value)
			PlayerHUIUserWidget->SetRemainAmmoVisible((float)(PlayerSaveGame->NumOfAmmoInChamber[PlayerSaveGame->EquippedGun] + PlayerSaveGame->NumOfAmmoInMagazine[PlayerSaveGame->EquippedGun]) / (GunData->ChamberCapacity + GunData->MagazineCapacity));
		else
			PlayerHUIUserWidget->SetRemainAmmoVisible(-1);


		GunAnim->PullBolt = Value;
	}
}

bool AWPPlayerActor::GetGunAnimInstanceBolt()
{
	if (PlayerSaveGame->EquippedGun == 0)
		return Cast<UWPGunAnimInstance>(MainGunMeshComponent->GetAnimInstance())->PullBolt;
	else
		return Cast<UWPGunAnimInstance>(SubGunMeshComponent->GetAnimInstance())->PullBolt;
}

void AWPPlayerActor::SavePlayerStat()
{
	PlayerSaveGame->MaxHP = MaxHP;
	PlayerSaveGame->CurrentHP = CurrentHP;
	UGameplayStatics::SaveGameToSlot(PlayerSaveGame, Cast<UWPGameInstance>(GetGameInstance())->PlayerSlot, 0);
}

bool AWPPlayerActor::IsRealesed()
{
	FName NowState = GetMesh()->GetAnimInstance()->GetStateMachineInstanceFromName(TEXT("UpperStateMachine"))->GetCurrentStateName();
	UWPPlayerAnimInstance* PlayerAnimInstance = Cast<UWPPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	return ( NowState == TEXT("Aiming") || NowState == TEXT("Running") ) && PlayerAnimInstance->PressBoltKey == 0 && PlayerAnimInstance->PressSwapKey == 0 && PlayerAnimInstance->TryToNormalReload == 0 && PlayerAnimInstance->TryToFastReload == 0;
}

void AWPPlayerActor::SetPlayerInputToFalsePerpect()
{
	SetPlayerInput(false);
	MoveDirection = FVector2D{ 0,0 };
	IsRunning = 0;
}

void AWPPlayerActor::SetPlayerInputToTruePerpect()
{
	if (!GetIsDead())
		SetPlayerInput(true);
}

void AWPPlayerActor::SetPlayerInput(bool Value)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (Value)
			EnableInput(PlayerController);
		else
			DisableInput(PlayerController);
	}
}

void AWPPlayerActor::OpenShop()
{
	if (IsRealesed())
	{
		SetPlayerInput(false);
		MoveDirection = FVector2D{ 0,0 };
		IsRunning = 0;
		//FillAmmo(0);
		//FillAmmo(1);
		PlayerHUIUserWidget->OpenShop();
	}
}

bool AWPPlayerActor::TryToSpendGold(uint32 Value)
{
	if (Value > PlayerSaveGame->Gold)
		return false;
	else 
	{
		PlayerSaveGame->Gold -= Value;
		return true;
	}
}

void AWPPlayerActor::ReceiveGold(uint32 Value)
{
	PlayerSaveGame->Gold += Value;
	PlayerSaveGame->CumulativeEXP += Value / 10.0f;
	if(GoldSound && Value > 0)
		UGameplayStatics::PlaySoundAtLocation(this, GoldSound, GetActorLocation());
}

void AWPPlayerActor::SetGunAsPurchase(FName GunName)
{
	PlayerSaveGame->GunPurchase[GunName] = 1;
}

bool AWPPlayerActor::TryToReceiveGun(FName GunName)
{
	if (IsRealesed())
	{
		ReceiveGun(GunName);
		return true;
	}

	return false;
}

bool AWPPlayerActor::TryToReceiveItem(FName ItemName,float Amount)
{
	int8 EmptyIndex = GetEmptyItemSpace();

	if (EmptyIndex == -1)
		return false;

	UWPItemDataAsset* ItemData = UWPItemDataAsset::GetItemData(ItemName);

	PlayerSaveGame->ItemsInGear[EmptyIndex] = ItemData;

	if (Amount == -1)
		PlayerSaveGame->ItemAmounts[EmptyIndex] = ItemData->InitialCapacity;
	else
		PlayerSaveGame->ItemAmounts[EmptyIndex] = Amount;

	PlayerHUIUserWidget->RefreshGear(2);

	return true;
}

void AWPPlayerActor::UpgradeGear()
{
	if(PlayerSaveGame->GearLevel < 2)
		PlayerSaveGame->GearLevel++;

	FillAmmo(0);
	FillAmmo(1);
}

int8 AWPPlayerActor::GetEmptyItemSpace()
{

	for (int i = 0; i < PlayerSaveGame->ItemsInGear.Num(); i++)
	{
		if(PlayerSaveGame->ItemsInGear[i] == nullptr)
			return i;
	}

	return -1;
}

void AWPPlayerActor::SetStatusEffect(FName StatusEffectName, float Value)
{
	PlayerSaveGame->StatusEffect[StatusEffectName] += Value;
	if (PlayerSaveGame->StatusEffect[StatusEffectName] < 0)
		PlayerSaveGame->StatusEffect[StatusEffectName] = 0;
}

void AWPPlayerActor::SetStaminaConsumption(float Consumption)
{
	PlayerSaveGame->StaminaConsumption = Consumption;
}

void AWPPlayerActor::SetMoveSpeed(float Ratio)
{
	PlayerSaveGame->WalkMaxSpeed = PlayerSaveGame->DefaultWalkMaxSpeed * Ratio;
	PlayerSaveGame->RunAcceleration = PlayerSaveGame->DefaultRunAcceleration * Ratio;
	PlayerSaveGame->RunMaxSpeed = PlayerSaveGame->DefaultRunMaxSpeed * Ratio;
	PlayerSaveGame->AimWalkMaxSpeed = PlayerSaveGame->DefaultAimWalkMaxSpeed * Ratio;
}

float AWPPlayerActor::GetMaxRunSpeed()
{
	if (!PlayerSaveGame)
		return 700;

	return PlayerSaveGame->RunMaxSpeed;
}

void AWPPlayerActor::GenerateRoomForMap(TArray<class UWPArena*> Arenas)
{
	if (PlayerHUIUserWidget)
		PlayerHUIUserWidget->GenerateRoomForMap(Arenas);
}

void AWPPlayerActor::GeneratePathForMap(TArray<FVector2D> Paths)
{
	if (PlayerHUIUserWidget)
		PlayerHUIUserWidget->GeneratePathForMap(Paths);
}

float AWPPlayerActor::GetCumulativeEXP()
{
	return PlayerSaveGame->CumulativeEXP;
}

void AWPPlayerActor::SetLoadingScreenToFalse()
{
	if (PlayerHUIUserWidget)
		PlayerHUIUserWidget->SetLoadingScreen(false);
}

UNiagaraSystem* AWPPlayerActor::GetDefaultHitEffect()
{
	return DefaultAttackHitEffect;
}

void AWPPlayerActor::SetRemainMagazineAmmoVisible(uint8 Y, uint8 X, bool Value)
{
	if (Y > 1)
		return;

	if (!Value)
	{
		PlayerHUIUserWidget->SetRemainMagazineAmmoVisible(-1);
		return;
	}

	UWPGunDataAsset* GunData = UWPGunDataAsset::GetGunData(PlayerSaveGame->PossessedGuns[Y]);
	UWPAmmoDataAsset* AmmoData = GunData->AmmoUsed;

	if (AmmoData->isMagazine)
	{
		if (Y == 0)
		{
			PlayerHUIUserWidget->SetRemainMagazineAmmoVisible((float)PlayerSaveGame->AmmoInGearUpper[X] / GunData->MagazineCapacity);
		}
		else 
		{
			PlayerHUIUserWidget->SetRemainMagazineAmmoVisible((float)PlayerSaveGame->AmmoInGearLower[X] / GunData->MagazineCapacity);
		}
	}

}

void AWPPlayerActor::LiftPathFog(FVector2D PathIndex)
{
	if (PlayerHUIUserWidget)
		PlayerHUIUserWidget->LiftPathFog(PathIndex);
}

void AWPPlayerActor::SetBossIcon(uint8 Index)
{
	if (PlayerHUIUserWidget)
		PlayerHUIUserWidget->SetBossIcon(Index);
}

void AWPPlayerActor::SetTutorialWidget(FString Id)
{
	if (PlayerHUIUserWidget)
		PlayerHUIUserWidget->SetTutorialWidget(Id);
}

void AWPPlayerActor::SetIsTrueAiming(uint8 value)
{
	IsTrueAiming = value;
}

FVector AWPPlayerActor::GetMouseAimPosition(float Height)
{
	FVector2D ScreenPosition;
	Cast<APlayerController>(GetController())->GetMousePosition(ScreenPosition.X, ScreenPosition.Y);


	FVector WorldPosition, WorldDirection;
	Cast<APlayerController>(GetController())->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldPosition, WorldDirection);

	if (WorldDirection.Z == 0)
	{
		WorldDirection.Z = -0.000001f;
		UE_LOG(LogPlayerManagement, Warning, TEXT("0 divide occurs in mouse coordinate extraction."));
	}

	float ZExp = (Height - WorldPosition.Z) / WorldDirection.Z;

	float X = WorldDirection.X * ZExp + WorldPosition.X;

	float Y = WorldDirection.Y * ZExp + WorldPosition.Y;

	return FVector(X,Y,Height);
}

void AWPPlayerActor::SetIsShotCoolTimeFalse()
{
	IsShotCoolTime = 0;
}

void AWPPlayerActor::SetCanRecoverStaminaTrue()
{
	CanRecoverStamina = 1;
}

void AWPPlayerActor::OpenPlayerDeadWidget()
{
	PlayerHUIUserWidget->SetEXPText(PlayerSaveGame->CumulativeEXP);
	PlayerHUIUserWidget->OpenPlayerDeadWidget();
}

void AWPPlayerActor::InputMoveY(float Value)
{
	MoveDirection.Y = Value;
}

void AWPPlayerActor::InputMoveX(float Value)
{
	MoveDirection.X = Value;
}

void AWPPlayerActor::PressRun()
{
	if(CanRun)
		IsRunning = 1;
}

void AWPPlayerActor::ReleaseRun()
{
	IsRunning = 0;
}

void AWPPlayerActor::PressBolt()
{
	Cast<UWPPlayerAnimInstance>(GetMesh()->GetAnimInstance())->PressBoltKey = 1;
}

void AWPPlayerActor::ReleaseBolt()
{
	Cast<UWPPlayerAnimInstance>(GetMesh()->GetAnimInstance())->PressBoltKey = 0;
}

void AWPPlayerActor::PressSwap()
{
	if (PlayerSaveGame->PossessedGuns[1 - PlayerSaveGame->EquippedGun] != TEXT(""))
		Cast<UWPPlayerAnimInstance>(GetMesh()->GetAnimInstance())->PressSwapKey = 1;
}

void AWPPlayerActor::ReleaseSwap()
{
	Cast<UWPPlayerAnimInstance>(GetMesh()->GetAnimInstance())->PressSwapKey = 0;
}

void AWPPlayerActor::PressShot()
{
	if (IsShotCoolTime == 0 && GetVelocity().Length() < GetBasedOnRunningSpeed() && GetMesh()->GetAnimInstance()->GetStateMachineInstanceFromName(TEXT("UpperStateMachine"))->GetCurrentStateName() == TEXT("Aiming"))
	{
		if (PlayerSaveGame->NumOfAmmoInChamber[PlayerSaveGame->EquippedGun] > 0)
		{
			UWPGunDataAsset* GunData = PlayerSaveGame->GetEquippedGunData();
			UWPAmmoDataAsset* AmmoData = GunData->AmmoUsed;

			UGameplayStatics::PlaySoundAtLocation(this, GunData->FireSound, GetActorLocation()); //sound
			UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.0f, this, 1100.0f);

			//effect

			USkeletalMeshComponent* GunMesh;
			if (PlayerSaveGame->EquippedGun == 0)
				GunMesh = MainGunMeshComponent;
			else
				GunMesh = SubGunMeshComponent;

			UNiagaraComponent* EffectComp = UNiagaraFunctionLibrary::SpawnSystemAttached(GunData->MuzzleFlashEffect, GunMesh, TEXT("MuzzleSocket"), FVector(0,0,0), FRotator(),EAttachLocation::KeepRelativeOffset, true);				
			
			if (EffectComp)
			{
				EffectComp->SetWorldLocation(GunMesh->GetSocketLocation(TEXT("MuzzleSocket")));
				EffectComp->SetWorldRotation(GunMesh->GetSocketRotation(TEXT("MuzzleSocket")));
			}

			//raycast shot
			FVector ShoulderPostion = GetActorLocation();
			ShoulderPostion.Z = ShoulderHeight;
			ShoulderPostion += GetActorRightVector() * ShoulderLength;

			FVector MouseAimPosition = GetMouseAimPosition(ShoulderHeight);

			const float AttackRange = 2200;

			for (int i = 0; i < AmmoData->NumOfWarheads; i++)
			{
				FVector RealAimPosition = MouseAimPosition - ShoulderPostion;
				if (RealAimPosition.X == 0 && RealAimPosition.Y == 0)
				{
					FVector Forward = GetActorForwardVector();
					RealAimPosition.X = Forward.X;
					RealAimPosition.Y = Forward.Y;
				}
				RealAimPosition = { RealAimPosition.Y,-RealAimPosition.X,RealAimPosition.Z };
				RealAimPosition.Normalize();
				RealAimPosition *= FMath::FRandRange(-GunData->Spread / 2, GunData->Spread / 2) * (MouseAimPosition - ShoulderPostion).Length();
				RealAimPosition += MouseAimPosition;
				RealAimPosition = RealAimPosition - ShoulderPostion;
				if (RealAimPosition.X == 0 && RealAimPosition.Y == 0)
				{
					FVector Forward = GetActorForwardVector();
					RealAimPosition.X = Forward.X;
					RealAimPosition.Y = Forward.Y;
				}
				RealAimPosition.Normalize();
				RealAimPosition = ShoulderPostion + RealAimPosition * AttackRange;


				FVector LastHitPosition;
				FVector MuzzlePosition = GunMesh->GetSocketLocation(TEXT("MuzzleSocket"));
				MuzzlePosition.Z = ShoulderPostion.Z;

				IWPAttackTargetInterface::RaycastAttack(this, ShoulderPostion, RealAimPosition, AmmoData->Damage, AmmoData->penetration, {}, { this }, {}, {}, &LastHitPosition);

				if (RayEffectActor && (ShoulderPostion - LastHitPosition).Length() > (ShoulderPostion - MuzzlePosition).Length())
				{
					if(AmmoData->NumOfWarheads <= 1)
						GetWorld()->SpawnActor<AActor>(RayEffectActor, FTransform((LastHitPosition - MuzzlePosition).Rotation(), (MuzzlePosition + LastHitPosition) / 2))->SetActorScale3D(FVector((MuzzlePosition - LastHitPosition).Length() / 100, RayEffectWidth, RayEffectWidth));
					else
						GetWorld()->SpawnActor<AActor>(RayEffectActor, FTransform((LastHitPosition - MuzzlePosition).Rotation(), (MuzzlePosition + LastHitPosition) / 2))->SetActorScale3D(FVector((MuzzlePosition - LastHitPosition).Length() / 100, RayEffectBuckShotWidth, RayEffectBuckShotWidth));
				}

				
			}

			

			//recoil

			FVector2D ScreenPosition;
			Cast<APlayerController>(GetController())->GetMousePosition(ScreenPosition.X, ScreenPosition.Y);

			int32 ViewPortX;
			int32 ViewPortY;
			Cast<APlayerController>(GetController())->GetViewportSize(ViewPortX,ViewPortY);

			ScreenPosition -= FVector2D{ViewPortX / 2.0f, ViewPortY / 2.0f};

			if (ScreenPosition == FVector2D{0, 0})
				ScreenPosition = { 1,1 };

			float HRecoil = FMath::FRandRange(-GunData->Recoil/2, GunData->Recoil/2) * ScreenPosition.Length() * (1 - PlayerSaveGame->RecoilControl);
			FVector2D HRecoilVector = { ScreenPosition.Y,-ScreenPosition.X };
			HRecoilVector.Normalize();
			HRecoilVector *= HRecoil;

			FVector2D VRecoilVector = ScreenPosition;
			VRecoilVector.Normalize();
			VRecoilVector *= GunData->Recoil * ScreenPosition.Length() * (1 - PlayerSaveGame->RecoilControl);
			
			ScreenPosition += HRecoilVector + VRecoilVector;
			ScreenPosition += FVector2D{ ViewPortX / 2.0f, ViewPortY / 2.0f };

			ScreenPosition.X = ScreenPosition.X < 0 ? 0 : ScreenPosition.X;
			ScreenPosition.Y = ScreenPosition.Y < 0 ? 0 : ScreenPosition.Y;

			ScreenPosition.X = ViewPortX < ScreenPosition.X ? ViewPortX : ScreenPosition.X;
			ScreenPosition.Y = ViewPortY < ScreenPosition.Y ? ViewPortY: ScreenPosition.Y;

			Cast<APlayerController>(GetController())->SetMouseLocation(ScreenPosition.X, ScreenPosition.Y);

			//set ammo

			if (GunData->DoAutoChamberLoad)
				ChamberLoad();
			else
				PlayerSaveGame->NumOfAmmoInChamber[PlayerSaveGame->EquippedGun]--;

			if (GunData->CanRackBolt && PlayerSaveGame->NumOfAmmoInChamber[PlayerSaveGame->EquippedGun] == 0)
				SetGunAnimInstanceBolt(true);

			//set cooltime

			if (GunData->ShotCoolTime > 0)
			{
				IsShotCoolTime = 1;
				GetWorldTimerManager().SetTimer(ShotCoolTimeHandle, this, &AWPPlayerActor::SetIsShotCoolTimeFalse, GunData->ShotCoolTime, false);
			}

		}
		else 
		{
			UGameplayStatics::PlaySoundAtLocation(this, EmptyAmmoSound, GetActorLocation());
		}
	}
}

void AWPPlayerActor::PressAim()
{
	if (IsTrueAiming)
	{
		SetIsTrueAiming(0);
		return;
	}

	if (!IsRunning)
		SetIsTrueAiming(1);
}

void AWPPlayerActor::PressInteraction()
{
	const float InteractionDistance = 200;

	/*
	FVector RaycastStart = CameraComponent->GetComponentLocation();
	FVector RaycastEnd = GetMouseAimPosition(0);

	TArray<FHitResult> HitResults;

	GetWorld()->LineTraceMultiByChannel(HitResults, RaycastStart, RaycastEnd, ECC_GameTraceChannel4);

	for (FHitResult HitResult : HitResults)
	{
		IWPInteractionInterface* InteractionInterface = Cast<IWPInteractionInterface>(HitResult.GetActor());
		if (InteractionInterface)
		{
			if ((HitResult.GetComponent()->GetComponentLocation() - GetActorLocation()).Length() <= InteractionDistance)
			{
				InteractionInterface->Interact(this);
				return;
			}
		}
	}
	*/

	TArray<FHitResult> HitResults2;
	GetWorld()->SweepMultiByChannel(HitResults2, GetActorLocation(), GetActorLocation(), FQuat(), ECC_GameTraceChannel4, FCollisionShape::MakeSphere(InteractionDistance));

	IWPInteractionInterface* InteractionTarget = nullptr;
	float Dis = 1000000;

	for (FHitResult HitResult : HitResults2)
	{
		IWPInteractionInterface* InteractionInterface = Cast<IWPInteractionInterface>(HitResult.GetActor());
		if (InteractionInterface)
		{
			float nowDis = (HitResult.GetComponent()->GetComponentLocation() - GetActorLocation()).Length();
			if (nowDis < Dis)
			{
				Dis = nowDis;
				InteractionTarget = InteractionInterface;
			}
		}
	}

	if (InteractionTarget)
		InteractionTarget->Interact(this);


}

void AWPPlayerActor::PressMap()
{
	if (PlayerHUIUserWidget)
		PlayerHUIUserWidget->ToggleMap();

	FVector2D ScreenPosition;
	Cast<APlayerController>(GetController())->GetMousePosition(ScreenPosition.X, ScreenPosition.Y);
	Cast<APlayerController>(GetController())->SetMouseLocation(ScreenPosition.X, ScreenPosition.Y);
}

void AWPPlayerActor::PressMenu()
{
	if (PlayerHUIUserWidget)
		PlayerHUIUserWidget->ToggleMainMenuPanel();

	FVector2D ScreenPosition;
	Cast<APlayerController>(GetController())->GetMousePosition(ScreenPosition.X, ScreenPosition.Y);
	Cast<APlayerController>(GetController())->SetMouseLocation(ScreenPosition.X, ScreenPosition.Y);
}




