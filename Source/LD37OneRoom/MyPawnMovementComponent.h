// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "MyPawnMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class LD37ONEROOM_API UMyPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};
