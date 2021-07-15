// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacterBase.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	EAI_Idle,
	EAI_Suspious,
	EAI_Chase,
	EAI_Attack
};


UCLASS()
class DEMO_PROJECT_API AAICharacterBase : public ACharacter
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, Category = "AISense")
		class UPawnSensingComponent* PawnSense;

	UPROPERTY(EditAnywhere, Category = "CollsionComp")
		class USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, Category = "CombatComp")
		class USphereComponent* CombatSphereComp;

	UPROPERTY(EditAnywhere, Category = "CombatComp")
		class USphereComponent* ProxSphereComp;

	EAIState GuardState;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		bool bIsPatrolling;
	bool bPawnSeen;

	FTimerHandle Handle_Move;

	bool bPlayerOverlaped;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void AttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void AttackEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SuspiousGuard();
	virtual void ChasePlayer();
	bool bIsChasing;
	bool bIsChaseRest;

	UPROPERTY(EditDefaultsOnly)
	float PatrolTime;

	void StopChasing();

	void ReturnPatrolling();
	FTimerHandle Handle_Patrol;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* Search_Mon;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* Attack_Mon;

	void OnAttack();
	void AttackStop();
	bool bIsAttacking;

	void FacePlayerDirection(APawn* Pwn);


public:
	// Sets default values for this character's properties
	AAICharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned

	UPROPERTY(VisibleAnywhere, Category = "AI", meta = (EditCondition = "bIsPatrolling"), meta = (AllowPrivateAccess = "true"))
		AActor* CurrentPatrolPoint;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (EditCondition = "bIsPatrolling"), meta = (AllowPrivateAccess = "true"))
		AActor* PatrolPointOne;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (EditCondition = "bIsPatrolling"), meta = (AllowPrivateAccess = "true"))
		AActor* PatrolPointTwo;


	UFUNCTION()
		virtual void ProxyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnPawnSeen(APawn* Pawn);

	void SetGuardState(EAIState NewState);


	void OnGuardStateChange(EAIState NewState);

	virtual void MoveAI();

};