// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PresentPawn.generated.h"

UCLASS()
class LD37ONEROOM_API APresentPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LD37", Meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent *StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LD37", Meta = (AllowPrivateAccess = "true"))
	class UMyPawnMovementComponent *MovComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LD37", Meta = (AllowPrivateAccess = "true"))
    float RandomStuff;

public:
	// Sets default values for this pawn's properties
	APresentPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
};
