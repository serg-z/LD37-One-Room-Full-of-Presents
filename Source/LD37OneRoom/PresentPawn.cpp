#include "LD37OneRoom.h"
#include "PresentPawn.h"

#include "MyPawnMovementComponent.h"
#include "BoyCharacter.h"

// Sets default values
APresentPawn::APresentPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	RootComponent = StaticMesh;
	
	MovComp = CreateDefaultSubobject<UMyPawnMovementComponent>(TEXT("MovComp"));
	MovComp->UpdatedComponent = RootComponent;

	m_materialBox = nullptr;
	m_materialRibbon = nullptr;

	{
		ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/LD37/Presents/Material_Box.Material_Box'"));

		if (Material.Succeeded())
			m_materialBox = Material.Object;
	}

	{
		ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/LD37/Presents/Material_Ribbon.Material_Ribbon'"));

		if (Material.Succeeded())
			m_materialRibbon = Material.Object;
	}
}

// Called when the game starts or when spawned
void APresentPawn::BeginPlay()
{
	Super::BeginPlay();

	SetRandomColors();
}

// Called every frame
//void APresentPawn::Tick( float DeltaTime )
//{
//	Super::Tick( DeltaTime );
//}

// Called to bind functionality to input
void APresentPawn::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("Move", this, &APresentPawn::MoveForward);
	InputComponent->BindAxis("Strafe", this, &APresentPawn::MoveSideways);
}

void APresentPawn::MoveForward(float Value)
{
	AddMovementInput(FVector(1, 0, 0), Value);
}

void APresentPawn::MoveSideways(float Value)
{
	AddMovementInput(FVector(0, 1, 0), Value);
}

void APresentPawn::SetRandomColors()
{
	if (m_materialBox)
	{
		UMaterialInstanceDynamic *mid = UMaterialInstanceDynamic::Create(m_materialBox, StaticMesh);
		mid->SetScalarParameterValue("Hue Shift", FMath::FRand());
		StaticMesh->SetMaterial(1, mid);
	}

	if (m_materialRibbon)
	{
		UMaterialInstanceDynamic *mid = UMaterialInstanceDynamic::Create(m_materialRibbon, StaticMesh);
		mid->SetScalarParameterValue("Hue Shift", FMath::FRand());
		StaticMesh->SetMaterial(0, mid);
	}
}