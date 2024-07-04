// Fill out your copyright notice in the Description page of Project Settings.


#include "TheText.h"

// Sets default values
ATheText::ATheText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ThreatText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ThreatTextNumber"));
	ThreatText->SetHorizontalAlignment(EHTA_Center);
	ThreatText->SetWorldSize(150.0f);
	RootComponent = ThreatText;
	ThreatText->SetRelativeLocation(FVector(0.0f,0.0f,200.0f));
}

// Called when the game starts or when spawned
void ATheText::BeginPlay()
{
	Super::BeginPlay();
	UpdateTimerDisplay();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATheText::AdvanceTimer, .5f, true);
}

// Called every frame
void ATheText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATheText::UpdateTimerDisplay()
{
	ThreatText->SetText(FText::FromString(FString::Printf(TEXT("%d %f"), ThreatLevel, AddFString)));
}

void ATheText::AddText(float TheAddFString)
{
	AddFString = TheAddFString;
}

void ATheText::AdvanceTimer()
{

	UpdateTimerDisplay();
	if (IsDeath)
	{
		//������ɣ�ֹͣ���ж�ʱ����
		GetWorldTimerManager().ClearTimer(TimerHandle);
		//��ʱ������ʱ��ִ��Ҫִ�е����������
		CountdownHasFinished();
	}
}

void ATheText::CountdownHasFinished()
{
	ThreatText->SetText(FText::FromString(FString(TEXT("Death"))));
}

void ATheText::SetThretLvel(int32 TheThreatLevel)
{
	ThreatLevel = TheThreatLevel;
}

void ATheText::Death()
{
	IsDeath = true;
}


