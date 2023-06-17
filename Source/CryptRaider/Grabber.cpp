// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"




// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (!PhysicsHandle)
	{
		return;
	}

	if (PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
	
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (!PhysicsHandle)
	{
		return;
	}
	UPrimitiveComponent* Component = PhysicsHandle->GetGrabbedComponent();
	if (Component)
	{
		Component->WakeAllRigidBodies();
		PhysicsHandle->ReleaseComponent();
		Component->GetOwner()->Tags.Remove("Grabbed");
	}
}

void UGrabber::Grab()
{
	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);
	if (HasHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Yellow, true, 5);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, true, 5);

		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		AActor* HitActor = HitResult.GetActor();
		HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitResult.GetComponent(),
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
	}
	
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle()const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!Result)
	{
		return nullptr;
	}
	else
	{
		return Result;
	}
}

bool UGrabber::GetGrabbableInReach(FHitResult& outHitResult)
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * TheMaxGrabDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, true, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;


	return GetWorld()->SweepSingleByChannel(
		outHitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		Sphere
	);

}
