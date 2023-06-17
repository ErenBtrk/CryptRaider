// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "Mover.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();


}


void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AActor* Actor = GetAcceptableActor();
	if (Actor)
	{
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if (Component)
		{
			Component->SetSimulatePhysics(false);
		}
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, FString::Printf(TEXT("Unlocking")));
		Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		if(Mover)
			Mover->SetShouldMove(true);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, FString::Printf(TEXT("Relocking")));
		if (Mover)
			Mover->SetShouldMove(false);
	}
}

AActor* UTriggerComponent::GetAcceptableActor()const
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);
	for (AActor* Actor : Actors)
	{
		if (Actor->ActorHasTag(AcceptableActorTag) && !Actor->ActorHasTag("Grabbed"))
		{
			return Actor;
		}
	}
	return nullptr;
}

void UTriggerComponent::SetMover(UMover* MoverToSet)
{
	Mover = MoverToSet;
}

