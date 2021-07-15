// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ADCharacter::ADCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/**Camera Boom Default Initialiser*/
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	/**Camera Component Default Initialiser*/
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation = false;

	BaseTurnRate = 45.0f;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	/**Charcter Configuration*/
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 275.0f;
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = .5f;

}

// Called when the game starts or when spawned
void ADCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADCharacter::MoveForward(float Val)
{
	const FRotator NewRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, NewRotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(Direction, Val);
}

void ADCharacter::MoveRight(float Val)
{
	const FRotator NewRotator = Controller->GetControlRotation();
	const FRotator YawRotation(0, NewRotator.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Direction, Val);

}

void ADCharacter::LookUp(float Val)
{
	AddControllerYawInput(Val * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADCharacter::LookRight(float Val)
{
	AddControllerPitchInput(Val * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

// Called to bind functionality to input
void ADCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ADCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ADCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &ADCharacter::LookRight);
}

