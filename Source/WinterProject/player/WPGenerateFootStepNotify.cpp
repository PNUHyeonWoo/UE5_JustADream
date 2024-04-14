// Fill out your copyright notice in the Description page of Project Settings.


#include "player/WPGenerateFootStepNotify.h"
#include "player/WPPlayerActor.h"
#include "Perception/AISense_Hearing.h"

void UWPGenerateFootStepNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AWPPlayerActor* PlayerActor = Cast<AWPPlayerActor>(MeshComp->GetOwner());

	if (PlayerActor)
	{
		float Speed = PlayerActor->GetVelocity().Length() / PlayerActor->GetMaxRunSpeed();
		if (Speed > 1)
			Speed = 1;

		const float maxRange = 800;


		UAISense_Hearing::ReportNoiseEvent(PlayerActor->GetWorld(), PlayerActor->GetActorLocation(), 1.0f, PlayerActor, maxRange* Speed);

	}


}
