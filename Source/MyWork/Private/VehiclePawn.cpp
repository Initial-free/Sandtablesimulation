// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn.h"

AVehiclePawn::AVehiclePawn()
{
    PrimaryActorTick.bCanEverTick = true;
    InitBasicMesh();
    InitBaseValue();
}

void AVehiclePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    int32 Select = FMath::Clamp(RootComponent->GetComponentRotation().Roll, -90.0f, 90.0f);
    if (Select == -90.0f || Select == 90.0f)
        InitialPosition();
}

void AVehiclePawn::CreateWheel(UStaticMeshComponent* StaticMesh, const FName& Name, const FVector Location, UStaticMesh* Mesh)
{
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(Name);
    StaticMesh->SetupAttachment(RootComponent);
    StaticMesh->SetStaticMesh(Mesh);
    StaticMesh->SetRelativeLocation(Location);
    StaticMesh->SetWorldScale3D(FVector(0.8f));
}

void AVehiclePawn::InitBasicMesh()
{
    //��ȡ�����ѡ��tank����
    int32 Choose = FMath::Clamp(FMath::RandRange(0, 100),30,31);
    if (Choose == 31)
        FirstTank();
    else
        SecondTank();
}

void AVehiclePawn::InitBaseValue()
{
    GunUp = 0.0f;
    Type = TargetType::TANK;
    int32 SelStatus = FMath::RandRange(1, 3);
    if (SelStatus == 1)
        Status = CombatStatus::STATUS_1;
    else if (SelStatus == 2)
        Status = CombatStatus::STATUS_2;
    else
        Status = CombatStatus::STATUS_3;
}

void AVehiclePawn::FirstTank()
{
    //��̨
    Battery = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BatteryVisual"));
    Battery->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BatteryVisual(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/T72/T-72-part_Turret.T-72-part_Turret'"));
    if (BatteryVisual.Succeeded())
    {
        Battery->SetStaticMesh(BatteryVisual.Object);
        Battery->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
        Battery->SetWorldScale3D(FVector(0.8f));
    }
    //����
    Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyVisual"));
    Body->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyVisual(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/T72/T-72-part_Body.T-72-part_Body'"));
    if (BodyVisual.Succeeded())
    {
        Body->SetStaticMesh(BodyVisual.Object);
        Body->SetRelativeLocation(FVector(0.0f, 0.0f, -60.0f));
        Body->SetWorldScale3D(FVector(0.8f));
    }

    //����
    TheFire = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireVisual"));
    TheFire->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> FireVisual(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/T72/T-72-part_gun.T-72-part_gun'"));
    if (FireVisual.Succeeded())
    {
        TheFire->SetStaticMesh(FireVisual.Object);
        TheFire->SetRelativeLocation(FVector(30.0f, 0.0f, -10.0f));
        TheFire->SetWorldScale3D(FVector(0.8f));
    }
    
    //����
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset_L(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/T72/T-72-part_wheel_LM.T-72-part_wheel_LM'"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset_R(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/T72/T-72-part_wheel_RM.T-72-part_wheel_RM'"));
    if (Asset_L.Succeeded() && Asset_R.Succeeded())
    {
        UStaticMeshComponent* LeftWheel[6] = {};
        UStaticMeshComponent* RightWheel[6] = {};
        for (int32 i = 0; i <= 5; i++)
        {
            FString Lstr = "LeftWheel", Rstr = "RightWheel";
            Lstr += FString::FromInt(i);
            Rstr += FString::FromInt(i);
            FVector Location(-160.0f + i * 60, -120.0f, -110.0f);
            CreateWheel(LeftWheel[i], FName(*Lstr), Location, Asset_L.Object);
            CreateWheel(RightWheel[i], FName(*Rstr), Location + FVector(0.0f, 240.0f, 0.0f), Asset_R.Object);
        }
    }
}

void AVehiclePawn::SecondTank()
{
    //��̨
    Battery = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BatteryVisual"));
    Battery->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BatteryVisual(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/M1/M1_parts_Turret.M1_parts_Turret'"));
    if (BatteryVisual.Succeeded())
    {
        Battery->SetStaticMesh(BatteryVisual.Object);
        Battery->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
        Battery->SetWorldScale3D(FVector(0.8f));
    }
    //����
    Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyVisual"));
    Body->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyVisual(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/M1/M1_parts_Body.M1_parts_Body'"));
    if (BodyVisual.Succeeded())
    {
        Body->SetStaticMesh(BodyVisual.Object);
        Body->SetRelativeLocation(FVector(0.0f, 0.0f, -85.0f));
        Body->SetWorldScale3D(FVector(0.8f));
    }
    //����
    TheFire = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireVisual"));
    TheFire->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> FireVisual(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/M1/M1_parts_Gun.M1_parts_Gun'"));
    if (FireVisual.Succeeded())
    {
        TheFire->SetStaticMesh(FireVisual.Object);
        TheFire->SetRelativeLocation(FVector(40.0f, 15.0f, 25.0f));
        TheFire->SetWorldScale3D(FVector(0.8f));
    }
    //����
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset_L(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/M1/M1_parts_Wheel_LF.M1_parts_Wheel_LF'"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset_R(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/M1/M1_parts_Wheel_RF.M1_parts_Wheel_RF'"));
    if (Asset_L.Succeeded() && Asset_R.Succeeded())
    {
        UStaticMeshComponent* LeftWheel[6] = {};
        UStaticMeshComponent* RightWheel[6] = {};
        for (int32 i = 0; i <= 5; i++)
        {
            FString Lstr = "LeftWheel", Rstr = "RightWheel";
            Lstr += FString::FromInt(i);
            Rstr += FString::FromInt(i);
            FVector Location(-160.0f + i * 60, -120.0f, -80.0f);
            CreateWheel(LeftWheel[i], FName(*Lstr), Location, Asset_L.Object);
            CreateWheel(RightWheel[i], FName(*Rstr), Location + FVector(0.0f, 240.0f, 0.0f), Asset_R.Object);
        }
    }
}

void AVehiclePawn::MakeFire()
{

    // ��ͼ���䷢����
    if (ProjectileClass)
    {
        // ��ȡ������任��
        FVector CameraLocation;
        FRotator CameraRotation;
        GetActorEyesViewPoint(CameraLocation, CameraRotation);

        // ����MuzzleOffset�����Կ��������ǰ���ɷ����
        MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

        // ��MuzzleOffset��������ռ�任������ռ䡣
        FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

        FRotator MuzzleRotation = CameraRotation;
        MuzzleRotation.Pitch += 10.0f;
        UWorld* World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            // ��ǹ��λ�����ɷ�����
            AWeapon* Projectile = World->SpawnActor<AWeapon>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
            if (Projectile)
            {
                // ���÷�����ĳ�ʼ�켣
                FVector LaunchDirection = MuzzleRotation.Vector();
                Projectile->FireInDirection(LaunchDirection);
            }
        }
    }
}

void AVehiclePawn::FireTurn(float AxisValue)
{ 
    GunUp = AxisValue * BaseTurnRate;
    FRotator spring_arm_rotator = TheFire->GetRelativeRotation();
    float spring_arm_pitch = spring_arm_rotator.Pitch;
    // ����Ƕ�С��-66.6��������ֵ�Ǹ��ģ���ִ����ת
    if (!(spring_arm_pitch < 0.0f && GunUp < 0.0f))
    {
        // ����Ƕȴ���0��������ֵ�����ģ���ִ����ת
        if (!(spring_arm_pitch > 60.0f && GunUp > 0.0f))
        {
            //ǹ����ת
            FRotator CameraRotator = FRotator(GunUp, 0.0f, 0.0f);
            TheFire->AddRelativeRotation(CameraRotator);
        }
    }
   
    
}
//��̨����ת
void AVehiclePawn::BatteryTurn(float AxisValue)
{
    float BatteryYaw = AxisValue;
    if (AxisValue)
        BatteryYaw = AxisValue * BaseTurnRate;
    FRotator BatteryRotator = FRotator(0.0f, BatteryYaw, 0.0f);
    Battery->AddRelativeRotation(BatteryRotator);
    TheFire->AddRelativeRotation(BatteryRotator);
    InternalCamera->AddRelativeRotation(BatteryRotator);
}
//������Ӧ
void AVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("GunPitch", this, &AVehiclePawn::FireTurn);
    PlayerInputComponent->BindAxis("BatteryTurn", this, &AVehiclePawn::BatteryTurn);
}
