// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DCharacter.generated.h"

UCLASS()
class DEMO_PROJECT_API ADCharacter : public ACharacter
{
	GENERATED_BODY()

		/**Variable for the Component*/
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraBoom", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
	float BaseTurnRate;

	void MoveForward(float Val);
	void MoveRight(float Val);
	void LookUp(float Val);
	void LookRight(float Val);

public:
	// Sets default values for this character's properties
	ADCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
