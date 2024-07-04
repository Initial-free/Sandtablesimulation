// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "BaseMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYWORK_API UBaseMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
public:
	//��д���ຯ��
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
