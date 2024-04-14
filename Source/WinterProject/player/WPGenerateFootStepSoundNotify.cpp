// Fill out your copyright notice in the Description page of Project Settings.


#include "player/WPGenerateFootStepSoundNotify.h"
#include "struct/WPfootStepComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"

void UWPGenerateFootStepSoundNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	FHitResult HitResult;
	
	if (MeshComp->GetWorld()->LineTraceSingleByChannel(HitResult, MeshComp->GetComponentLocation(), MeshComp->GetComponentLocation() - FVector(0, 0, 3000), ECC_GameTraceChannel6))
	{
		UWPfootStepComponent* FootStepComp = HitResult.GetActor()->GetComponentByClass<UWPfootStepComponent>();
		if (FootStepComp)
		{
			if(FootStepComp->FootStepSound)
				UGameplayStatics::PlaySoundAtLocation(MeshComp, FootStepComp->FootStepSound, HitResult.ImpactPoint);
		}
	}

}
