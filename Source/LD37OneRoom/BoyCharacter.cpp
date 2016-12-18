#include "LD37OneRoom.h"
#include "BoyCharacter.h"

#include "Engine.h"

#include "PresentPawn.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, text)

// Sets default values
ABoyCharacter::ABoyCharacter() :
	Pushing(false),
	PushingMoveTransition(0.0f)
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

	if (PushingPawn && Pushing)
	{
		PushingLastInputVector = PushingPawn->GetMovementComponent()->GetLastInputVector();

		{
			FVector v = PushingLastInputVector;
			v.Normalize();

			float alpha = 10.0f * DeltaTime;

			// exponential moving average
			PushingMoveTransition = alpha * v.Size() + (1.0f - alpha) * PushingMoveTransition;
		}

		// trace collision
		{
			const float rad = GetCapsuleComponent()->GetScaledCapsuleRadius();
			const float radExt = rad * 1.2f;

			//GetWorld()->DebugDrawTraceTag = TEXT("Yolo");

			ECollisionChannel colchan = ECC_Pawn;

			m_traceXPos = GetWorld()->LineTraceTestByChannel(GetActorLocation() + FVector(0.0f, rad, 0.0f), GetActorLocation() + FVector(radExt, rad, 0.0f), colchan, m_traceParams)
				|| GetWorld()->LineTraceTestByChannel(GetActorLocation() + FVector(0.0f, -rad, 0.0f), GetActorLocation() + FVector(radExt, -rad, 0.0f), colchan, m_traceParams);

			m_traceXNeg = GetWorld()->LineTraceTestByChannel(GetActorLocation() + FVector(0.0f, rad, 0.0f), GetActorLocation() + FVector(-radExt, rad, 0.0f), colchan, m_traceParams)
				|| GetWorld()->LineTraceTestByChannel(GetActorLocation() + FVector(0.0f, -rad, 0.0f), GetActorLocation() + FVector(-radExt, -rad, 0.0f), colchan, m_traceParams);

			m_traceYPos = GetWorld()->LineTraceTestByChannel(GetActorLocation() + FVector(rad, 0.0f, 0.0f), GetActorLocation() + FVector(rad, radExt, 0.0f), colchan, m_traceParams)
				|| GetWorld()->LineTraceTestByChannel(GetActorLocation() + FVector(-rad, 0.0f, 0.0f), GetActorLocation() + FVector(-rad, radExt, 0.0f), colchan, m_traceParams);

			m_traceYNeg = GetWorld()->LineTraceTestByChannel(GetActorLocation() + FVector(rad, 0.0f, 0.0f), GetActorLocation() + FVector(rad, -radExt, 0.0f), colchan, m_traceParams)
				|| GetWorld()->LineTraceTestByChannel(GetActorLocation() + FVector(-rad, 0.0f, 0.0f), GetActorLocation() + FVector(-rad, -radExt, 0.0f), colchan, m_traceParams);
		}
	}
}

// Called to bind functionality to input
void ABoyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Strafe", this, &ABoyCharacter::MoveSideways);
	PlayerInputComponent->BindAxis("Move", this, &ABoyCharacter::MoveForward);

	PlayerInputComponent->BindAction("Push", IE_Pressed, this, &ABoyCharacter::Push);
	PlayerInputComponent->BindAction("Push", IE_Released, this, &ABoyCharacter::Release);
}

void ABoyCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	if (PushingPawn && Pushing)
	{
		if (PushingPawn && Pushing)
		{
			FVector v = WorldDirection * ScaleValue;

			if ((m_traceXPos && v.X > 0) || (m_traceXNeg && v.X < 0))
				WorldDirection.X = 0;

			if ((m_traceYPos && v.Y) > 0 || (m_traceYNeg  && v.Y < 0))
				WorldDirection.Y = 0;

			PushingPawn->AddMovementInput(WorldDirection, ScaleValue, bForce);
		}
	}
	else
	{
		ACharacter::AddMovementInput(WorldDirection, ScaleValue, bForce);
	}
}

void ABoyCharacter::MoveSideways(float Value)
{
	AddMovementInput(FVector(0.f, 1.f, 0.f), Value);
}

void ABoyCharacter::MoveForward(float Value)
{
	AddMovementInput(FVector(1.f, 0.f, 0.f), Value);
}

void ABoyCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	APresentPawn *pawn = Cast<APresentPawn>(OtherActor);
	
	if (pawn && !Pushing)
	{
		PushingPawn = pawn;
	}
}

void ABoyCharacter::Push()
{
	if (PushingPawn && GetDistanceTo(PushingPawn) < 95.0f)
	{
		Pushing = true;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->MaxWalkSpeed = 150.f;

		AttachToActor(PushingPawn, FAttachmentTransformRules::KeepWorldTransform);
		SetActorEnableCollision(false);

		m_traceParams = FCollisionQueryParams(TEXT("Yolo"), true, this);
		m_traceParams.bReturnPhysicalMaterial = false;
		m_traceParams.AddIgnoredActor(PushingPawn);
	}
}

void ABoyCharacter::Release()
{
	Pushing = false;
	PushingMoveTransition = 0.0f;

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorEnableCollision(true);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}
