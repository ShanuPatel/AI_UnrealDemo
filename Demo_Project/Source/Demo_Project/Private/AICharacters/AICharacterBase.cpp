// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacters/AICharacterBase.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Character/DCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AAICharacterBase::AAICharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensingComp"));
	PawnSense->OnSeePawn.AddDynamic(this, &AAICharacterBase::OnPawnSeen);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereComp->SetupAttachment(GetMesh());

	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	CombatSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphereComp->SetupAttachment(GetMesh());

	CombatSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CombatSphereComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	CombatSphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatSphereComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	ProxSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("ProxySphere"));
	ProxSphereComp->SetupAttachment(GetMesh());

	ProxSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProxSphereComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	ProxSphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ProxSphereComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	ProxSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AAICharacterBase::ProxyBeginOverlap);

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AAICharacterBase::BeginOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &AAICharacterBase::EndOverlap);

	GuardState = EAIState::EAI_Idle;
	bIsPatrolling = false;
	bPawnSeen = false;
	bPlayerOverlaped = false;
	bIsChasing = false;
	bIsChaseRest = false;
	bIsAttacking = false;
	PatrolTime = 10.0f;

}

// Called when the game starts or when spawned
void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if (bIsPatrolling)
		MoveAI();

}

void AAICharacterBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, (TEXT("Overlaped called")))
			ADCharacter* MyPawn = Cast<ADCharacter>(OtherActor);
		if (MyPawn)
		{
			UE_LOG(LogTemp, Warning, (TEXT("Seen Overlaped called")))
				bPlayerOverlaped = true;

		}
	}
}

void AAICharacterBase::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bPlayerOverlaped && bIsChaseRest)
	{
		bPlayerOverlaped = false;
		GetWorldTimerManager().SetTimer(Handle_Move, this, &AAICharacterBase::StopChasing, 5.0f);
		UE_LOG(LogTemp, Warning, (TEXT("EndOverlap called")))

	}
}

void AAICharacterBase::AttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{

		UE_LOG(LogTemp, Warning, (TEXT("Attack called")))
			ADCharacter* MyPawn = Cast<ADCharacter>(OtherActor);
		if (MyPawn)
		{
			OnAttack();
		}
	}
}

void AAICharacterBase::AttackEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, (TEXT("Overlaped called")))
			ADCharacter* MyPawn = Cast<ADCharacter>(OtherActor);
		if (MyPawn)
		{
			UAnimInstance* AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
			if (AnimInstance)
			{
				AnimInstance->StopAllMontages(.2f);
			}
			ChasePlayer();
		}
	}
}

void AAICharacterBase::OnAttack()
{
	AController* AICon = GetController();
	AICon->StopMovement();
	if (!bIsAttacking)
	{
		bIsAttacking = true;
		UAnimInstance* AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(Attack_Mon, 1.0f);
			GetWorldTimerManager().SetTimer(Handle_Move, this, &AAICharacterBase::AttackStop, 1.8f);
		}
	}
}

void AAICharacterBase::AttackStop()
{
	AController* AICon = GetController();
	AICon->StopMovement();
	bIsAttacking = false;
	ACharacter* MyPawn = UGameplayStatics::GetPlayerCharacter(this, 0);
	FacePlayerDirection(MyPawn);
}

void AAICharacterBase::ProxyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{

		UE_LOG(LogTemp, Warning, (TEXT("Proxy called")))
			ADCharacter* MyPawn = Cast<ADCharacter>(OtherActor);
		if (MyPawn)
		{
			AController* Ai = GetController();
			Ai->StopMovement();
			FacePlayerDirection(MyPawn);
		}
	}
}

void AAICharacterBase::SuspiousGuard()
{

	AController* AI = GetController();
	AI->StopMovement();

	if (!bIsChasing && bPawnSeen)
	{
		UE_LOG(LogTemp, Warning, (TEXT("suspious called")))
			bIsChasing = true;
		GetWorldTimerManager().SetTimer(Handle_Move, this, &AAICharacterBase::ChasePlayer, 5.0f);
	}
}

void AAICharacterBase::ChasePlayer()
{
	UE_LOG(LogTemp, Warning, (TEXT("Chasing called")))
	if (bPlayerOverlaped)
	{
		SetGuardState(EAIState::EAI_Chase);
		ACharacter* MyPawn = UGameplayStatics::GetPlayerCharacter(this, 0);
		AAIController* AIControl = Cast<AAIController>(GetController());
		if (AIControl)
		{
			bIsChaseRest = true;

			GetCharacterMovement()->MaxWalkSpeed = 200.0;
			AIControl->MoveToLocation(MyPawn->GetActorLocation(), 100.0f);

		}
	}
}

void AAICharacterBase::StopChasing()
{
	UE_LOG(LogTemp, Warning, TEXT("StopedChasing"));
	AController* AI = GetController();
	AI->StopMovement();
	bIsChasing = false;
	bIsChaseRest = false;
	bPawnSeen = false;
	UAnimInstance* AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(Search_Mon, 1.0f);
	}
	GetWorldTimerManager().SetTimer(Handle_Patrol, this, &AAICharacterBase::ReturnPatrolling, PatrolTime);

}

void AAICharacterBase::ReturnPatrolling()
{
	UE_LOG(LogTemp, Warning, TEXT("Retrun"));
	if(!bPawnSeen)
	MoveAI();
}

void AAICharacterBase::OnPawnSeen(APawn* Pawn)
{
	bPawnSeen = true;

	ACharacter* MyPawn = UGameplayStatics::GetPlayerCharacter(this, 0);
	AAIController* AIControl = Cast<AAIController>(GetController());
	if (AIControl && bPlayerOverlaped && !bIsChasing)
	{
		UE_LOG(LogTemp, Warning, (TEXT("Seen called")))
			SuspiousGuard();

	}
	else
	{
		if (bIsChaseRest)
		{
			if (AIControl)
			{
				bIsChaseRest = true;
				UE_LOG(LogTemp, Warning, (TEXT("Pawn Chasing called")))
				GetCharacterMovement()->MaxWalkSpeed = 200.0;
				AIControl->MoveToActor(MyPawn, 50.0f);
			}
		}

	}

	FacePlayerDirection(MyPawn);

	SetGuardState(EAIState::EAI_Suspious);

	DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 20.0f, 12, FColor::Black, false, 10.0f);

}

void AAICharacterBase::FacePlayerDirection(APawn* Pwn)
{
	FVector Direction = Pwn->GetActorLocation() - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAtLocation = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAtLocation.Pitch = 0.0f;
	NewLookAtLocation.Roll = 0.0f;
	SetActorRotation(NewLookAtLocation);
}

void AAICharacterBase::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}
	GuardState = NewState;
	OnGuardStateChange(GuardState);
}

void AAICharacterBase::OnGuardStateChange(EAIState NewState)
{
	//UI Changes to be Applies with use of Enum
}

void AAICharacterBase::MoveAI()
{
	SetGuardState(EAIState::EAI_Idle);
	GetCharacterMovement()->MaxWalkSpeed = 220.0;
	if (!bPawnSeen) {
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

	GetWorldTimerManager().SetTimer(Handle_Move, this, &AAICharacterBase::MoveAI, 10.0f);
}

// Called every frame
void AAICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


