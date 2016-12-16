#pragma once

#include "GameFramework/Pawn.h"
#include "PresentPawn.generated.h"

UCLASS()
class LD37ONEROOM_API APresentPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LD37", Meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent *StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LD37", Meta = (AllowPrivateAccess = "true"))
	class UMyPawnMovementComponent *MovComp;

public:
	// Sets default values for this pawn's properties
	APresentPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	//virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

protected:
	void MoveForward(float Value);
	void MoveSideways(float Value);

	void SetRandomColors();

	UMaterial *m_materialBox;
	UMaterial *m_materialRibbon;
};
