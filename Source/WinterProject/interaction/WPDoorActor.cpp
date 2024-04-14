// Fill out your copyright notice in the Description page of Project Settings.


#include "interaction/WPDoorActor.h"
#include "player//WPPlayerActor.h"
#include "enemyUnit//WPBaseEnemyUnit.h"
#include <Kismet/GameplayStatics.h>

AWPDoorActor::AWPDoorActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RotationAngle = 140;
	RotationSpeed = 180;

	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseComponent"));
	RootComponent = BaseComponent;
	AxisComponent = CreateDefaultSubobject<USceneComponent>(TEXT("AxisComponent"));
	AxisComponent->SetupAttachment(RootComponent);
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(AxisComponent);

	MeshComponent->SetCanEverAffectNavigation(false);

	SightComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SightComponent"));
	SightComponent->SetupAttachment(MeshComponent);

	SightComponent->SetVisibility(false);
}

void AWPDoorActor::Interact(AWPPlayerActor* PlayerActor)
{
	if (RotationDirection != 0)
		return;

	if (bIsOpen)
	{
		float Yaw = AxisComponent->GetRelativeRotation().Yaw;
		RotationDirection = Yaw > 0 ? -1 : 1;
		bIsOpen = 0;

		if (NavMeshComponent)
			NavMeshComponent->SetCanEverAffectNavigation(false);
	}
	else 
	{
		RotationDirection = GetDoorToActorDirection(PlayerActor);
		bIsOpen = 1;

		if(OpenSound)
			UGameplayStatics::PlaySoundAtLocation(this, OpenSound, GetActorLocation());
	}


}

void AWPDoorActor::BeginPlay()
{
	Super::BeginPlay();
	SightComponent->SetVisibility(true);
}

void AWPDoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RotationDirection != 0)
	{
		AxisComponent->SetRelativeRotation(AxisComponent->GetRelativeRotation() + FRotator(0, RotationDirection * RotationSpeed * DeltaTime, 0));

		float Yaw = AxisComponent->GetRelativeRotation().Yaw;

		if (bIsOpen)
		{
			if (RotationDirection > 0 && Yaw > RotationAngle)
			{
				RotationDirection = 0;
				AxisComponent->SetRelativeRotation(FRotator(AxisComponent->GetRelativeRotation().Pitch, RotationAngle, AxisComponent->GetRelativeRotation().Roll));
				if (NavMeshComponent)
					NavMeshComponent->SetCanEverAffectNavigation(true);
			}
			else if (RotationDirection < 0 && Yaw < -RotationAngle)
			{
				RotationDirection = 0;
				AxisComponent->SetRelativeRotation(FRotator(AxisComponent->GetRelativeRotation().Pitch, -RotationAngle, AxisComponent->GetRelativeRotation().Roll));
				if (NavMeshComponent)
					NavMeshComponent->SetCanEverAffectNavigation(true);
			}
		}
		else 
		{
			if (RotationDirection > 0 && Yaw > 0)
			{
				RotationDirection = 0;
				AxisComponent->SetRelativeRotation(FRotator(AxisComponent->GetRelativeRotation().Pitch, 0, AxisComponent->GetRelativeRotation().Roll));
				if(CloseSound)
					UGameplayStatics::PlaySoundAtLocation(this, CloseSound, GetActorLocation());
			}
			else if (RotationDirection < 0 && Yaw < 0)
			{
				RotationDirection = 0;
				AxisComponent->SetRelativeRotation(FRotator(AxisComponent->GetRelativeRotation().Pitch, 0, AxisComponent->GetRelativeRotation().Roll));
				if (CloseSound)
					UGameplayStatics::PlaySoundAtLocation(this, CloseSound, GetActorLocation());
			}
		}

	}

}

void AWPDoorActor::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (RotationDirection != 0 || bIsOpen == 1)
		return;

	if (Cast<AWPBaseEnemyUnit>(Other))
	{
		RotationDirection = GetDoorToActorDirection(Other);
		bIsOpen = 1;
		if (OpenSound)
			UGameplayStatics::PlaySoundAtLocation(this, OpenSound, GetActorLocation());
	}
}

float AWPDoorActor::GetDoorToActorDirection(AActor* Target)
{
	FVector DoorToActor3D = Target->GetActorLocation() - MeshComponent->GetComponentLocation();
	FVector2D DoorToActor2D = { DoorToActor3D.X,DoorToActor3D.Y };
	DoorToActor2D.Normalize();
	FVector2D RightVector = { GetActorRightVector().X,GetActorRightVector().Y};
	RightVector.Normalize();

	return FVector2D::DotProduct(DoorToActor2D,RightVector) > 0 ? -1 : 1;
}
