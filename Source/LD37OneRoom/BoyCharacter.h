#pragma once

#include "GameFramework/Character.h"
#include "BoyCharacter.generated.h"

UCLASS()
class LD37ONEROOM_API ABoyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Pushing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector PushingLastInputVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PushingMoveTransition;

	// Sets default values for this character's properties
	ABoyCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void MoveSideways(float Val);
	void MoveForward(float Val);
	void MovePushingPawn(FVector dir, float Val);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	void Push();
	void Release();

	class APresentPawn *PushingPawn;
};
