// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "FighterPawn.generated.h"
/**
 * 
 */

UCLASS()
class MYWORK_API AFighterPawn : public ABasePawn
{
	GENERATED_BODY()
public:
	AFighterPawn();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void InitBasicMesh();
	UFUNCTION()
	void InitBasicValue();
	UFUNCTION()
	void FirstFighter();
	UFUNCTION()
	void SecondFighter();
	UFUNCTION()
	void Update();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(VisibleAnywhere, Category = "Fighter", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Parts;
	UPROPERTY(VisibleAnywhere, Category = "Fighter", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Fuselage;

	//起飞时的位置
	UPROPERTY(VisibleAnywhere)
	FVector FighterLocation;
	UPROPERTY(VisibleAnywhere)
	float GunUp;

	// 假设有一个变量来表示飞机是否已经起飞
	bool bIsAirborne = false;

	// 物理属性
	float Mass = 1.0f; // 飞机质量
	float Drag = 0.1f; // 阻力系数
	float Lift = 1000.0f; // 升力系数

	int64 CountdownTime;
public:
	//Fighter的操控
	//上升
	UFUNCTION()
	void FighterRise(float AxisValue);

	
	void Turn(float AxisValue);

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
