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
    
    //获取随机数选择Fighter生成
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
    //机身
    Fuselage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FuselageVisual"));
    Fuselage->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> FuselageVisual(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/SU-57/SU-57_SU57-airframe.SU-57_SU57-airframe'"));
    if (FuselageVisual.Succeeded())
    {
        Fuselage->SetStaticMesh(FuselageVisual.Object);
        Fuselage->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
        Fuselage->SetWorldScale3D(FVector(0.08f));
    }
    //零件
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
    //机身
    Fuselage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FuselageVisual"));
    Fuselage->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> FuselageVisual(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/SpaceFighter/Cube_011.Cube_011'"));
    if (FuselageVisual.Succeeded())
    {
        Fuselage->SetStaticMesh(FuselageVisual.Object);
        Fuselage->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
        Fuselage->SetWorldScale3D(FVector(50.0f));
    }
    //零件
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
        // 平滑过渡，使用FVector的Lerp函数来插值当前位置和目标位置
        FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, GetWorld()->DeltaTimeSeconds, 10.0f);
        // 应用新的位置
        RootComponent->SetWorldLocation(NewLocation, true, nullptr, ETeleportType::None);
    }
    else if (TargetLocation.Z > 6000.0f)
    {
        TargetLocation.Z = 6000.0f;
        // 平滑过渡，使用FVector的Lerp函数来插值当前位置和目标位置
        FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, GetWorld()->DeltaTimeSeconds, -10.0f);
        // 应用新的位置
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
        // 如果飞机已经起飞，根据AxisValue判断上升下降
        if (AxisValue != 0.0f)
        {
            // 计算升力
            float LiftForce = Lift * AxisValue;
            //获取当前位置
            FighterLocation = GetActorLocation();
            // 应用升力
            FVector TargetLocation = GetActorLocation();
            TargetLocation.Z += LiftForce / Mass;

            if (AxisValue > 0.0f) {
                // 旋转组件，使其往上翘
                FRotator TargetRotation = FRotator(FMath::Lerp(0.0f, 25.0f, 2*AxisValue), GetActorRotation().Yaw, GetActorRotation().Roll);
                FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->DeltaTimeSeconds, 10.0f);
                RootComponent->SetRelativeRotation(NewRotation, true, nullptr, ETeleportType::None);
                 // 平滑过渡，使用FVector的Lerp函数来插值当前位置和目标位置
                FVector NewLocation = FMath::VInterpTo(GetActorLocation() , TargetLocation, GetWorld()->DeltaTimeSeconds, 5.0f);
                // 应用新的位置
                RootComponent->SetWorldLocation(NewLocation, true, nullptr, ETeleportType::None);
            }
            else
            {
                // 平滑过渡，使用FVector的Lerp函数来插值当前位置和目标位置
                FVector NewLocation = FMath::VInterpTo(GetActorLocation(),TargetLocation , GetWorld()->DeltaTimeSeconds, 1.0f);
                // 应用新的位置
                RootComponent->SetWorldLocation(NewLocation, true, nullptr, ETeleportType::None);
                // 旋转组件，使其往下翘
                FRotator TargetRotation = FRotator(FMath::Lerp(0.0f, -25.0f, 2*AxisValue), GetActorRotation().Yaw, GetActorRotation().Roll);
                FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->DeltaTimeSeconds, 10.0f);
                RootComponent->SetRelativeRotation(NewRotation, true, nullptr, ETeleportType::None);
                
            }
        }
        else
        {
            // 当不上升时，组件回到原位
            FRotator TargetRotation = FRotator(0.0f, GetActorRotation().Yaw, GetActorRotation().Roll);
            FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->DeltaTimeSeconds, 10.0f);
            RootComponent->SetWorldRotation(NewRotation, true, nullptr, ETeleportType::None);

            FVector TargetLocation = FVector(GetActorLocation().X, GetActorLocation().Y, FighterLocation.Z);
            RootComponent->SetWorldLocation(TargetLocation, true, nullptr, ETeleportType::None);
            // 当不上升时，维持当前高度
        }
    }
}

void AFighterPawn::Turn(float AxisValue)
{
    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += AxisValue;
    if (CameraFllow)
    {
        //如果摄像机跟随则全部旋转
        SetActorRotation(NewRotation);
        //重写父类转弯函数
        //添加旋转时有反转Roll
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
