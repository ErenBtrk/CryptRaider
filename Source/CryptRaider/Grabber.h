// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Grabber.generated.h"

class UPhysicsHandleComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void Release();
	UFUNCTION(BlueprintCallable)
		void Grab();
	UFUNCTION(BlueprintCallable)
		bool GetGrabbableInReach(FHitResult& outHitResult);
private:
	UPROPERTY(EditAnywhere)
		float TheMaxGrabDistance = 400;
	UPROPERTY(EditAnywhere)
		float GrabRadius = 100;
	UPROPERTY(EditAnywhere)
		float HoldDistance = 200;

	UPhysicsHandleComponent* GetPhysicsHandle()const;
};
