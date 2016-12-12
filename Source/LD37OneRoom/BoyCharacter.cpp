// Fill out your copyright notice in the Description page of Project Settings.

#include "LD37OneRoom.h"
#include "BoyCharacter.h"

#include "PresentPawn.h"

// Sets default values
ABoyCharacter::ABoyCharacter() :
	m_pushing(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ABoyCharacter::OnHit);
}

// Called when the game starts or when spawned
void ABoyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoyCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (m_boxPawn)
	{
		if (GetDistanceTo(m_boxPawn) > 95)
		{
			Release();
			return;
		}

		if (m_pushing)
		{
			SetActorLocation(m_boxPawn->GetActorLocation() + m_offset);
		}
	}
}

// Called to bind functionality to input
void ABoyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Strafe", this, &ABoyCharacter::Strafe);
	PlayerInputComponent->BindAxis("Move", this, &ABoyCharacter::Move);

	PlayerInputComponent->BindAction("Push", IE_Pressed, this, &ABoyCharacter::Push);
	PlayerInputComponent->BindAction("Push", IE_Released, this, &ABoyCharacter::Release);
}

void ABoyCharacter::Strafe(float Value)
{
	const FVector &v = FVector(0.f, 1.f, 0.f);

	AddMovementInput(v, Value);

	MoveBox(v, Value);
}

void ABoyCharacter::Move(float Value)
{
	const FVector &v = FVector(1.f, 0.f, 0.f);

	AddMovementInput(v, Value);

	MoveBox(v, Value);
}

void ABoyCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	APresentPawn *pawn = Cast<APresentPawn>(OtherActor);
	
	if (pawn && !m_pushing)
	{
		m_boxPawn = pawn;
	}
}

void ABoyCharacter::Push()
{
	UE_LOG(LogTemp, Warning, TEXT("PUSH"));

	if (m_boxPawn)
	{
		m_pushing = true;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->MaxWalkSpeed = 150.f;

		m_offset = GetActorLocation() - m_boxPawn->GetActorLocation();
		m_offset.Normalize();
		m_offset *= 90;
	}
}

void ABoyCharacter::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("RELEASE"));

	m_pushing = false;
	m_boxPawn = nullptr;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void ABoyCharacter::MoveBox(const FVector &v, float Value)
{
	if (m_boxPawn && m_pushing)
	{
		m_boxPawn->AddMovementInput(v, Value);
	}
}