// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacters/CowardAICharacter.h"
#include "GameFramework/Actor.h"
#include "Character/DCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

void ACowardAICharacter::ProxyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::ProxyBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor)
	{

		UE_LOG(LogTemp, Warning, (TEXT("Child Proxy called")))
			ADCharacter* MyPawn = Cast<ADCharacter>(OtherActor);
		if (MyPawn)
		{
			UE_LOG(LogTemp, Warning, (TEXT("Child CLassChasing called")))
			MoveAI();
		}
	}
}

void ACowardAICharacter::MoveAI()
{
	if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint == PatrolPointTwo)
	{
		CurrentPatrolPoint = PatrolPointOne;
		UE_LOG(LogTemp, Warning, (TEXT("First called")))

	}
	else
	{
		CurrentPatrolPoint = PatrolPointTwo;
		UE_LOG(LogTemp, Warning, (TEXT("second called")))

	}
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);
}

void ACowardAICharacter::OnPawnSeen(APawn* Pawn)
{
}

void ACowardAICharacter::BeginPlay()
{
	Super::BeginPlay();
}
