// Fill out your copyright notice in the Description page of Project Settings.

#include "LD37OneRoom.h"
#include "PresentPawn.h"

#include "MyPawnMovementComponent.h"

// Sets default values
APresentPawn::APresentPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	RootComponent = StaticMesh;
	
	MovComp = CreateDefaultSubobject<UMyPawnMovementComponent>(TEXT("MovComp"));
	MovComp->UpdatedComponent = RootComponent;
	
	// ribbon color
	{
		static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/LD37/Presents/Material_Ribbon.Material_Ribbon'"));

		if (Material.Succeeded())
		{
			UMaterialInstanceDynamic *mid = UMaterialInstanceDynamic::Create(Material.Object, StaticMesh);
			mid->SetScalarParameterValue("Hue Shift", FMath::FRand());
			StaticMesh->SetMaterial(0, mid);
		}
	}

	// box color
	{
		static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/LD37/Presents/Material_Box.Material_Box'"));

		if (Material.Succeeded())
		{
			UMaterialInstanceDynamic *mid = UMaterialInstanceDynamic::Create(Material.Object, StaticMesh);
			mid->SetScalarParameterValue("Hue Shift", FMath::FRand());
			StaticMesh->SetMaterial(1, mid);
		}
	}
}

// Called when the game starts or when spawned
void APresentPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APresentPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

// Called to bind functionality to input
void APresentPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

