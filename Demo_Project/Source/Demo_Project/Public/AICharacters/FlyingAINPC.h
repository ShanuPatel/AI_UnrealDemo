// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AICharacters/AICharacterBase.h"
#include "FlyingAINPC.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_PROJECT_API AFlyingAINPC : public AAICharacterBase
{
	GENERATED_BODY()
protected:
		virtual void BeginPlay() override;
};
