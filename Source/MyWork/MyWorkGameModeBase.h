// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VehiclePawn.h"
#include "FighterPawn.h"
#include "ArmoredCarPawn.h"
#include "BasePawn.h"
#include "TheText.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "MyWorkGameModeBase.generated.h"
UCLASS()
class MYWORK_API AMyWorkGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyWorkGameModeBase();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameMode", meta = (AllowPrivateAccess = "true"))
	TArray<ABasePawn*> EnemyG;

	int64 Totalnum;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Initialization();
	void TheControl();
	void SetTimer();
	void TheCalculation();

	//���
	ABasePawn* MyPawn;

};
