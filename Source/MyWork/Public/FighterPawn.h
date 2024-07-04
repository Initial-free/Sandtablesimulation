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

	//���ʱ��λ��
	UPROPERTY(VisibleAnywhere)
	FVector FighterLocation;
	UPROPERTY(VisibleAnywhere)
	float GunUp;

	// ������һ����������ʾ�ɻ��Ƿ��Ѿ����
	bool bIsAirborne = false;

	// ��������
	float Mass = 1.0f; // �ɻ�����
	float Drag = 0.1f; // ����ϵ��
	float Lift = 1000.0f; // ����ϵ��

	int64 CountdownTime;
public:
	//Fighter�Ĳٿ�
	//����
	UFUNCTION()
	void FighterRise(float AxisValue);

	
	void Turn(float AxisValue);

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
