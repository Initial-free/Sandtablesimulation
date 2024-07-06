// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmoredCarPawn.h"
//��ʼ��װ�׳�
AArmoredCarPawn::AArmoredCarPawn()
{
	InitBasicMesh();
	InitBaseValue();
}
//ѭ��ִ��װ�׳�������

void AArmoredCarPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	int32 Select = FMath::Clamp(RootComponent->GetComponentRotation().Roll, -90.0f, 90.0f);
	if (Select == -90.0f || Select == 90.0f)
		InitialPosition();
}
void AArmoredCarPawn::CreateWheel(UStaticMeshComponent* StaticMesh, const FName& Name, const FVector Location, UStaticMesh* Mesh)
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(Name);
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetStaticMesh(Mesh);
	StaticMesh->SetRelativeLocation(Location);
	StaticMesh->SetWorldScale3D(FVector(0.8f));
}
//��ʼ����ͬ�����װ�׳�
void AArmoredCarPawn::InitBasicMesh()
{
    //��һ������
	FirstArmoredCar();
    //��n������
    //......
}

//��һ������
void AArmoredCarPawn::InitBasicMesh()
{
	FirstArmoredCar();
}
void AArmoredCarPawn::FirstArmoredCar()
{

    //����
    Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyVisual"));
    Body->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyVisual(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/LUV351/LUV_3151_SK_East_LUV_3151_LOD0_001.LUV_3151_SK_East_LUV_3151_LOD0_001'"));
    if (BodyVisual.Succeeded())
    {
        Body->SetStaticMesh(BodyVisual.Object);
        Body->SetRelativeLocation(FVector(0.0f, 0.0f, -60.0f));
        Body->SetWorldScale3D(FVector(0.8f));
    }


    //����
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset_L(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/LUV351/LUV_3151_Wheels_L.LUV_3151_Wheels_L'"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset_R(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/LUV351/LUV_3151_Wheels_R.LUV_3151_Wheels_R'"));
    if (Asset_L.Succeeded() && Asset_R.Succeeded())
    {
        UStaticMeshComponent* LeftWheel[2] = {};
        UStaticMeshComponent* RightWheel[2] = {};
        for (int32 i = 0; i <= 1; i++)
        {
            FString Lstr = "LeftWheel", Rstr = "RightWheel";
            Lstr += FString::FromInt(i);
            Rstr += FString::FromInt(i);
            FVector Location(-90.0f + i * 195.0f, -50.0f, -50.0f);
            CreateWheel(LeftWheel[i], FName(*Lstr), Location, Asset_L.Object);
            CreateWheel(RightWheel[i], FName(*Rstr), Location + FVector(0.0f, 100.0f, 0.0f), Asset_R.Object);
        }
    }
}

//��ʼ����ֵ
void AArmoredCarPawn::InitBaseValue()
{
    //��ʼ����������
    Type = TargetType::ARMORED_CAR;
    //��ʼ��״̬
}
void AArmoredCarPawn::InitBaseValue()
{
    Type = TargetType::ARMORED_CAR;
    int32 SelStatus = FMath::RandRange(1, 3);
    if (SelStatus == 1)
        Status = CombatStatus::STATUS_1;
    else if(SelStatus == 2)
        Status = CombatStatus::STATUS_2;
    else
        Status = CombatStatus::STATUS_3;
}
