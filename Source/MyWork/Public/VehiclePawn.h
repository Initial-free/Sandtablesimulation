// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "VehiclePawn.generated.h"

/**
 * 
 */
UCLASS()
class MYWORK_API AVehiclePawn : public ABasePawn
{
	GENERATED_BODY()
public:
	AVehiclePawn();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void CreateWheel(UStaticMeshComponent* StaticMesh, const FName& Name, const FVector Location, UStaticMesh* Mesh);
	UFUNCTION()
	void InitBasicMesh();
	UFUNCTION()
	void InitBaseValue();
	UFUNCTION()
	void FirstTank();
	UFUNCTION()
	void SecondTank();
public:
	//×é¼þ
	UPROPERTY(VisibleAnywhere, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Battery;
	UPROPERTY(VisibleAnywhere, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Body;
	UPROPERTY(VisibleAnywhere, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TheFire;

	UPROPERTY(VisibleAnywhere)
	float GunUp;

public:
	UFUNCTION()
	void MakeFire();
	//tankµÄ²Ù¿Ø
	UFUNCTION()
	void FireTurn(float AxisValue);
	UFUNCTION()
	void BatteryTurn(float AxisValue);

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
