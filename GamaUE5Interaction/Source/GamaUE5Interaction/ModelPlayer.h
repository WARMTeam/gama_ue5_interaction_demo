// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ModelPlayer.generated.h"

UCLASS()
class GAMAUE5INTERACTION_API AModelPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AModelPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComp;

	// Parameters for movements and camera zoom
	FVector MovementInput;
	FVector CameraInput;
	float ZoomFactor;
	bool bZoomingIn;
	float MovingSpeed = 10000.0f;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Basic directional movements for the player
	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);

	// Basic camera movements for the camera
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);

	// Zooming in and out in the map
	void ZoomIn();
	void ZoomOut();
};
