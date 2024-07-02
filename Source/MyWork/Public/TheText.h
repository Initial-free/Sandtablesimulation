// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/TextRenderComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TheText.generated.h"

UCLASS()
class MYWORK_API ATheText : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATheText();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//倒数的运行时长（以秒计）
	UPROPERTY(EditAnywhere)
	bool IsDeath = false;

	float AddFString;
	int ThreatLevel;
	UTextRenderComponent* ThreatText;


	void UpdateTimerDisplay();
	void AddText(float AddFString);
	void AdvanceTimer();

	void CountdownHasFinished();
	void SetThretLvel(int32 TheThreatLevel);
	void Death();

	FTimerHandle TimerHandle;
};
