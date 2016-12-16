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

void ABoyCharacter::MoveSideways(float Value)
{
	const FVector &v = FVector(0.f, 1.f, 0.f);

	if (PushingPawn && Pushing)
	{
		MovePushingPawn(v, Value);
	}
	else
	{
		AddMovementInput(v, Value);
	}
}

void ABoyCharacter::MoveForward(float Value)
{
	const FVector &v = FVector(1.f, 0.f, 0.f);

	if (PushingPawn && Pushing)
	{
		MovePushingPawn(v, Value);
	}
	else
	{
		AddMovementInput(v, Value);
	}
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

void ABoyCharacter::MovePushingPawn(FVector v, float Value)
{
	if (PushingPawn && Pushing)
	{
		PushingPawn->AddMovementInput(v, Value);
	}
}