// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "ArmoredCarPawn.generated.h"

/**
 * 
 */
UCLASS()
class MYWORK_API AArmoredCarPawn : public ABasePawn
{
	GENERATED_BODY()
public:
	AArmoredCarPawn(); 
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void CreateWheel(UStaticMeshComponent* StaticMesh, const FName& Name, const FVector Location, UStaticMesh* Mesh);
	UFUNCTION()
	void InitBasicMesh();
	UFUNCTION()
	void FirstArmoredCar();
	UFUNCTION()
	void InitBaseValue();

	UPROPERTY(VisibleAnywhere, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Body;
};
