// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AICharacters/AICharacterBase.h"
#include "CowardAICharacter.generated.h"

/**
 *
 */
UCLASS()
class DEMO_PROJECT_API ACowardAICharacter : public AAICharacterBase
{
	GENERATED_BODY()
		//virtual void ChasePlayer()override;

	virtual void ProxyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	virtual void MoveAI();

	virtual void OnPawnSeen(APawn* Pawn)override;

protected:
	virtual void BeginPlay() override;
};
