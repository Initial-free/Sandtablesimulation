// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterPawn.h"
#include <Components/SphereComponent.h>

AFighterPawn::AFighterPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    InitBasicMesh();
    InitBasicValue();
}

void AFighterPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AFighterPawn::InitBasicMesh()
{
    
    //��ȡ�����ѡ��Fighter����
    int32 Choose = FMath::Clamp(FMath::RandRange(0, 100), 30, 31);
    if (!IsPendingKill())
    {
        TArray<UPrimitiveComponent*> Components;
        GetComponents(Components);
        const UPrimitiveComponent* RootPrimComp = Cast<UPrimitiveComponent>(AFighterPawn::GetRootComponent());
        bool bFoundPrimitiveRoot = (RootPrimComp != nullptr);

        for (UPrimitiveComponent* Component : Components)
        {
            if (!bFoundPrimitiveRoot && RootComponent->GetOwner() == this)
            {
                RootComponent = Component;
                bFoundPrimitiveRoot = true;
            }

            Component->SetSimulatePhysics(false);
        }
    }
    if (Choose == 31)
        FirstFighter();
    else
        SecondFighter();
   
}

void AFighterPawn::InitBasicValue()
{
    CountdownTime = 100;
    bIsAirborne = true;
    Mass = 1.0f;
    Drag = 0.1f;
    Lift = 1000.0f;

    Type = TargetType::PLANE;
    int32 SelStatus = FMath::RandRange(1, 3);
    if (SelStatus == 1)
        Status = CombatStatus::STATUS_1;
    else if (SelStatus == 2)
        Status = CombatStatus::STATUS_2;
    else
        Status = CombatStatus::STATUS_3;
}

void AFighterPawn::FirstFighter()
{
    //����
    Fuselage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FuselageVisual"));
    Fuselage->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> FuselageVisual(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/SU-57/SU-57_SU57-airframe.SU-57_SU57-airframe'"));
    if (FuselageVisual.Succeeded())
    {
        Fuselage->SetStaticMesh(FuselageVisual.Object);
        Fuselage->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
        Fuselage->SetWorldScale3D(FVector(0.08f));
    }
    //���
    Parts = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PartsVisual"));
    Parts->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> PartsVisual(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/SU-57/SU-57_SU57-landingOn.SU-57_SU57-landingOn'"));
    if (PartsVisual.Succeeded())
    {
        Parts->SetStaticMesh(PartsVisual.Object);
        Parts->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
        Parts->SetWorldScale3D(FVector(0.08f));
    }
}

void AFighterPawn::SecondFighter()
{
    //����
    Fuselage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FuselageVisual"));
    Fuselage->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> FuselageVisual(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/SpaceFighter/Cube_011.Cube_011'"));
    if (FuselageVisual.Succeeded())
    {
        Fuselage->SetStaticMesh(FuselageVisual.Object);
        Fuselage->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
        Fuselage->SetWorldScale3D(FVector(50.0f));
    }
    //���
    Parts = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PartsVisual"));
    Parts->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> PartsVisual(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/SpaceFighter/Cube_012.Cube_012'"));
    if (PartsVisual.Succeeded())
    {
        Parts->SetStaticMesh(PartsVisual.Object);
        Parts->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
        Parts->SetWorldScale3D(FVector(50.0f));
    }
}

void AFighterPawn::BeginPlay()
{
    Super::BeginPlay();
}
void AFighterPawn::Update()
{
    
    FVector TargetLocation = GetActorLocation();
    if (TargetLocation.Z < 2000.0f)
    {
        TargetLocation.Z = 2000.0f;
        // ƽ�����ɣ�ʹ��FVector��Lerp��������ֵ��ǰλ�ú�Ŀ��λ��
        FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, GetWorld()->DeltaTimeSeconds, 10.0f);
        // Ӧ���µ�λ��
        RootComponent->SetWorldLocation(NewLocation, true, nullptr, ETeleportType::None);
    }
    else if (TargetLocation.Z > 6000.0f)
    {
        TargetLocation.Z = 6000.0f;
        // ƽ�����ɣ�ʹ��FVector��Lerp��������ֵ��ǰλ�ú�Ŀ��λ��
        FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, GetWorld()->DeltaTimeSeconds, -10.0f);
        // Ӧ���µ�λ��
        RootComponent->SetWorldLocation(NewLocation, true, nullptr, ETeleportType::None);
    }
    else
    {
        FVector Location = FVector(GetActorLocation().X, GetActorLocation().Y, FighterLocation.Z);
        FVector NewLocation = FMath::VInterpTo(GetActorLocation(), Location, GetWorld()->DeltaTimeSeconds, -1.0f);
        RootComponent->SetWorldLocation(NewLocation, true, nullptr, ETeleportType::None);
    }
    InitialPosition();
}
void AFighterPawn::FighterRise(float AxisValue)
{
    if (bIsAirborne)
    {
        // ����ɻ��Ѿ���ɣ�����AxisValue�ж������½�
        if (AxisValue != 0.0f)
        {
            // ��������
            float LiftForce = Lift * AxisValue;
            //��ȡ��ǰλ��
            FighterLocation = GetActorLocation();
            // Ӧ������
            FVector TargetLocation = GetActorLocation();
            TargetLocation.Z += LiftForce / Mass;

            if (AxisValue > 0.0f) {
                // ��ת�����ʹ��������
                FRotator TargetRotation = FRotator(FMath::Lerp(0.0f, 25.0f, 2*AxisValue), GetActorRotation().Yaw, GetActorRotation().Roll);
                FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->DeltaTimeSeconds, 10.0f);
                RootComponent->SetRelativeRotation(NewRotation, true, nullptr, ETeleportType::None);
                 // ƽ�����ɣ�ʹ��FVector��Lerp��������ֵ��ǰλ�ú�Ŀ��λ��
                FVector NewLocation = FMath::VInterpTo(GetActorLocation() , TargetLocation, GetWorld()->DeltaTimeSeconds, 5.0f);
                // Ӧ���µ�λ��
                RootComponent->SetWorldLocation(NewLocation, true, nullptr, ETeleportType::None);
            }
            else
            {
                // ƽ�����ɣ�ʹ��FVector��Lerp��������ֵ��ǰλ�ú�Ŀ��λ��
                FVector NewLocation = FMath::VInterpTo(GetActorLocation(),TargetLocation , GetWorld()->DeltaTimeSeconds, 1.0f);
                // Ӧ���µ�λ��
                RootComponent->SetWorldLocation(NewLocation, true, nullptr, ETeleportType::None);
                // ��ת�����ʹ��������
                FRotator TargetRotation = FRotator(FMath::Lerp(0.0f, -25.0f, 2*AxisValue), GetActorRotation().Yaw, GetActorRotation().Roll);
                FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->DeltaTimeSeconds, 10.0f);
                RootComponent->SetRelativeRotation(NewRotation, true, nullptr, ETeleportType::None);
                
            }
        }
        else
        {
            // ��������ʱ������ص�ԭλ
            FRotator TargetRotation = FRotator(0.0f, GetActorRotation().Yaw, GetActorRotation().Roll);
            FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->DeltaTimeSeconds, 10.0f);
            RootComponent->SetWorldRotation(NewRotation, true, nullptr, ETeleportType::None);

            FVector TargetLocation = FVector(GetActorLocation().X, GetActorLocation().Y, FighterLocation.Z);
            RootComponent->SetWorldLocation(TargetLocation, true, nullptr, ETeleportType::None);
            // ��������ʱ��ά�ֵ�ǰ�߶�
        }
    }
}

void AFighterPawn::Turn(float AxisValue)
{
    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += AxisValue;
    if (CameraFllow)
    {
        //��������������ȫ����ת
        SetActorRotation(NewRotation);
        //��д����ת�亯��
        //�����תʱ�з�תRoll
        if (AxisValue != 0)
        {
            FRotator TargetRotation = FRotator(0.0f,
                GetActorRotation().Yaw,
                AxisValue > 0 ? FMath::Lerp(0.0f, 10.0f, AxisValue) : FMath::Lerp(0.0f, -10.0f, -AxisValue));
            RootComponent->SetRelativeRotation(TargetRotation);
        }
        else
        {
            FRotator TargetRotation = FRotator(0.0f ,GetActorRotation().Yaw,0);
            RootComponent->SetRelativeRotation(TargetRotation);
        }
    }
    else
    {
        if (AxisValue)
        {
            SetActorRotation(NewRotation);
        }
    }
    
}

void AFighterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("FighterRise", this, &AFighterPawn::FighterRise);
    PlayerInputComponent->BindAxis("Turn", this, &AFighterPawn::Turn);
}
