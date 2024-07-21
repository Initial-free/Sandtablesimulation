// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <Kismet/GameplayStatics.h>

ABasePawn::ABasePawn()
{
    
    ProjectileClass = AWeapon::StaticClass();
    InitBaseValue();
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    // 控制默认玩家
    //AutoPossessPlayer = EAutoReceiveInput::Player0;
    // 根组件将成为对物理反应的球体
    USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
    RootComponent = SphereComponent;
    SphereComponent->InitSphereRadius(80.0f);
    SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
    SphereComponent->SetEnableGravity(true);
    SphereComponent->SetSimulatePhysics(true);

    InitCameraComponent();
    if(TheText!=NULL)
    {
        TheText = GetWorld()->SpawnActor<ATheText>(GetActorLocation(), FRotator::ZeroRotator);
        TheText->ThreatText->SetupAttachment(RootComponent);
    }
    
    
}


void ABasePawn::InitBaseValue()
{
    //摄像机的基本数据
    BaseTurnRate = 5;
    BaseLookUpRate = 1.0f;
    LookUpInput = 0.0f;
    CameraType = true;
    CameraFllow = true;
    //初始位置
    InitialRotation = GetActorRotation();
    Threatlevel = 0;
    //初始化威胁程度
    Radius = FMath::RandRange(400.0f,600.0f);
    EmInterference1 = FMath::RandRange(200.0f, 400.0f);
    EmInterference2 = FMath::RandRange(400.0f, 700.0f);
    IsDeath = false;
    EnemyPitch = 0;
}

void ABasePawn::InitCameraComponent()
{

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);

    // 设置相对位置
    SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
    SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

    SpringArm->TargetArmLength = 700.f;

    SpringArm->bInheritPitch = false;
    SpringArm->bInheritYaw = true;
    SpringArm->bInheritRoll = false;

    SpringArm->bEnableCameraLag = true;
    SpringArm->bEnableCameraRotationLag = true;
    SpringArm->CameraLagSpeed = 8.0f;
    SpringArm->CameraRotationLagSpeed = 2.5f;

    // 创建摄像机并附加到弹簧臂
    ExternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ExternalCamera"));
    ExternalCamera->FieldOfView = 90.0f;
    ExternalCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    OurMovementComponent = CreateDefaultSubobject<UBaseMovementComponent>(TEXT("CustomMovementComponent"));
    OurMovementComponent->UpdatedComponent = RootComponent;
    //设置车内相机的位置
    InternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InternalCamera"));
    // 大体位置，不同类型车 蓝图调整位置
    InternalCamera->SetRelativeLocation(FVector(9.25f, -35.7f, 125.8f));
    InternalCamera->FieldOfView = 110.0f;
    InternalCamera->SetupAttachment(RootComponent);
}
PRAGMA_DISABLE_OPTIMIZATION
// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
    while(TheText==NULL|| TheText->ThreatText==NULL)
         TheText = GetWorld()->SpawnActor<ATheText>(GetActorLocation(), FRotator::ZeroRotator);
}
PRAGMA_ENABLE_OPTIMIZATION
// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (TheText && TheText->ThreatText)
    {
        TheText->ThreatText->SetRelativeLocation(GetActorLocation() + FVector(0.0f, 0.0f, 200.0f));
    }
    IsItOutOfBounds();
}
//进行按键绑定
// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABasePawn::GunFire);
    PlayerInputComponent->BindAction("StopCameraFollow", IE_Pressed, this, &ABasePawn::CameraFollow);
    PlayerInputComponent->BindAction("ChangeCamera", IE_Pressed, this, &ABasePawn::VehicleChangeCamera);
    PlayerInputComponent->BindAction("Initial", IE_Pressed, this, &ABasePawn::InitialPosition);
    PlayerInputComponent->BindAxis("moveForward", this, &ABasePawn::MoveForward);
    PlayerInputComponent->BindAxis("Turn", this, &ABasePawn::Turn);
    PlayerInputComponent->BindAxis("LRTranslation", this, &ABasePawn::CameraTurnAround);
    PlayerInputComponent->BindAxis("LookUp", this, &ABasePawn::CameraLookUp);
    PlayerInputComponent->BindAxis("Break", this, &ABasePawn::HandBrake);
}
//获取前进模组
UPawnMovementComponent* ABasePawn::GetMovementComponent() const
{
    return OurMovementComponent;
}
//改变驾车视角
void ABasePawn::VehicleChangeCamera()
{
    switch (CameraType)
    {
    case false:
        ExternalCamera->Activate(true);
        InternalCamera->Deactivate();
        break;
    case true:
        InternalCamera->Activate(true);
        ExternalCamera->Deactivate();
        break;
    }
    CameraType = !CameraType;
}
//摄像臂的伸缩
void ABasePawn::CameraSpringArmZoomIn()
{
    if (SpringArm->TargetArmLength > 300.0f)
    {
        SpringArm->TargetArmLength = SpringArm->TargetArmLength - 20.0f;
    }
}

void ABasePawn::CameraSpringArmZoomOut()
{
    //如果弹簧臂小，则增加
    if (SpringArm->TargetArmLength < 1000.f)
    {
        SpringArm->TargetArmLength = SpringArm->TargetArmLength + 20.0f;
    }
}
//摄像头单独旋转
void ABasePawn::CameraTurnAround(float AxisValue)
{
    if(!CameraFllow)
    {
        //如果摄像机不跟随则单独旋转
        float CameraYaw = AxisValue;
        if (AxisValue)
            CameraYaw = AxisValue * BaseTurnRate;
        FRotator CameraRotator = FRotator(0.0f, CameraYaw, 0.0f);
        InternalCamera->AddRelativeRotation(CameraRotator);
        SpringArm->AddRelativeRotation(CameraRotator);
    }
}
//转弯
void ABasePawn::Turn(float AxisValue)
{
    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += AxisValue;
    if (CameraFllow)
    {
        //如果摄像机跟随则全部旋转
        SetActorRotation(NewRotation);
    }
    else
    {
        if (AxisValue)
        {
            SetActorRotation(NewRotation);
        }
    }
}
//摄像头抬起
void ABasePawn::CameraLookUp(float AxisValue)
{
    
    //LookUpInput = axis_value * BaseLookUpRate * GetWorld()->DeltaTimeSeconds;
    LookUpInput = AxisValue * BaseLookUpRate;
   
    // Pitch 旋转限制
    FRotator spring_arm_rotator = SpringArm->GetRelativeRotation();
    float spring_arm_pitch = spring_arm_rotator.Pitch;
    // 如果当前俯仰角度小于-66.6度且输入值是负的，则不执行旋转
    if (!(spring_arm_pitch < -66.6f && LookUpInput < 0.0f))
    {
        // 如果当前俯仰角度大于0度且输入值是正的，则不执行旋转
        if (!(spring_arm_pitch > 0.0f && LookUpInput > 0.0f))
        {
            //相机的上下转
            FRotator CameraRotator = FRotator(LookUpInput, 0.0f, 0.0f);
            InternalCamera->AddRelativeRotation(CameraRotator);
            SpringArm->AddRelativeRotation(CameraRotator);
        }
    }
}
//摄像头跟踪
void ABasePawn::CameraFollow()
{
    CameraFllow = !CameraFllow;
    if (CameraFllow)
    {
        //如果此时跟随，则将摄像机的相对旋转减掉
        FRotator NewRotation = GetActorRotation();
       
        SpringArm->SetRelativeRotation(NewRotation);
        InternalCamera->SetRelativeRotation(NewRotation);
        SpringArm->SetWorldRotation(NewRotation);
        InternalCamera->SetWorldRotation(NewRotation);
    }
}
//向前移动
void ABasePawn::MoveForward(float AxisValue)
{
    if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
    {
        if (AxisValue != 0) // 按键按下
        {
            // 加速
            accumulatedAcceleration += FMath::Abs(AxisValue) * GetWorld()->GetDeltaSeconds() * AccelerationRate;
            accumulatedAcceleration = FMath::Clamp(accumulatedAcceleration, 0.0f, accelerationTime);
            CurrentSpeed = FMath::Lerp(0, MaxSpeed, accumulatedAcceleration / accelerationTime);
        }
        else
        {
            // 减速
            accumulatedAcceleration -= GetWorld()->GetDeltaSeconds();
            accumulatedAcceleration = FMath::Clamp(accumulatedAcceleration, 0.0f, accelerationTime);
            CurrentSpeed = FMath::Lerp(0, MaxSpeed, accumulatedAcceleration / accelerationTime);
        }

        // 确保速度在合理范围内
        CurrentSpeed = FMath::Clamp(CurrentSpeed, 0.0f, MaxSpeed);

        OurMovementComponent->AddInputVector(GetActorForwardVector()* CurrentSpeed);
    }
}
//随机转弯
void ABasePawn::RandomTurn()
{
    static bool isTure = false;
    static double OldAxisValue = 0;;
    FRotator NewRotation = GetActorRotation();
    if (isTure)
    {
        NewRotation.Yaw += OldAxisValue * 20;
    }
    else
    { 
        double AxisValue = FMath::Clamp(FMath::RandRange(-10, 10), -1, 1);
        NewRotation.Yaw += AxisValue * 20;
        OldAxisValue = AxisValue;
        isTure = true;
    }
    if (CameraFllow)
    {
        //如果摄像机跟随则全部旋转
        // 获取当前旋转
        FRotator CurrentRotation = GetActorRotation();

        // 计算插值速度，可以根据需要调整
        float InterpSpeed = 10.0f;

        // 在当前旋转和目标旋转之间插值
        FRotator InterpolatedRotation = FMath::RInterpTo(CurrentRotation, NewRotation, GetWorld()->GetDeltaSeconds(), InterpSpeed);
        if (InterpolatedRotation == NewRotation)
            isTure = false;
        // 设置插值后的旋转
        SetActorRotation(InterpolatedRotation);
    }
    else
    {
        if (OldAxisValue!=0)
        {
            // 获取当前旋转
            FRotator CurrentRotation = GetActorRotation();

            // 计算插值速度，可以根据需要调整
            float InterpSpeed = 10.0f;

            // 在当前旋转和目标旋转之间插值
            FRotator InterpolatedRotation = FMath::RInterpTo(CurrentRotation, NewRotation, GetWorld()->GetDeltaSeconds(), InterpSpeed);

            // 设置插值后的旋转
            SetActorRotation(InterpolatedRotation);
        }
    }
}
//随机向前移动
void ABasePawn::RandomMoveForward()
{
    if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
    {
        accumulatedAcceleration += FMath::Abs(1) * GetWorld()->GetDeltaSeconds() * AccelerationRate;
        accumulatedAcceleration = FMath::Clamp(accumulatedAcceleration, 0.0f, accelerationTime);
        CurrentSpeed = FMath::Lerp(0, MaxSpeed, accumulatedAcceleration / accelerationTime);
        CurrentSpeed = FMath::RandRange(CurrentSpeed-4.0f, CurrentSpeed+4.0f);

        // 确保速度在合理范围内
        CurrentSpeed = FMath::Clamp(CurrentSpeed, 0.0f, MaxSpeed);
        OurMovementComponent->AddInputVector(GetActorForwardVector() * CurrentSpeed);
    }
}
//加速时间
float ABasePawn::GetDecelerationTime(float currentSpeed) const
{
    float timeConstant = 1.0f / (CurrentSpeed + 1.0f); // 时间常数，速度越高，时间越长
    return timeConstant;
}
void ABasePawn::GunFire()
{
    // 试图发射发射物
    if (ProjectileClass)
    {
        // 获取摄像机变换。
        FVector CameraLocation;
        FRotator CameraRotation;
        GetActorEyesViewPoint(CameraLocation, CameraRotation);

        // 设置MuzzleOffset，在略靠近摄像机前生成发射物。
        MuzzleOffset.Set(200.0f, 0.0f, 0.0f);

        // 将MuzzleOffset从摄像机空间变换到世界空间。
        FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

        // 使目标方向略向上倾斜。
        FRotator MuzzleRotation = CameraRotation;
        MuzzleRotation.Pitch += EnemyPitch;

        UWorld* World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            // 在枪口位置生成发射物
            AWeapon* Projectile = World->SpawnActor<AWeapon>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
            if (Projectile)
            {
                // 设置发射物的初始轨迹
                FVector LaunchDirection = MuzzleRotation.Vector();
                Projectile->FireInDirection(LaunchDirection);
            }
        }
    }
}
//设置高度仰角
void ABasePawn::SetEnemyPitch(float Temp)
{
    EnemyPitch = Temp;
}
//更新状态
void ABasePawn::InitialPosition()
{
    SetActorRotation(InitialRotation);
}
//自我销毁
void ABasePawn::ToDeath()
{
    this->TheText->Destroy();
    this->Destroy();
}

//刹车系统
void ABasePawn::HandBrake(float AxisValue)
{
    if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
    {
        if(AxisValue)
        {
            // 减速
            accumulatedAcceleration -= GetWorld()->GetDeltaSeconds()*5;
            accumulatedAcceleration = FMath::Clamp(accumulatedAcceleration, 0.0f, accelerationTime);
            CurrentSpeed = FMath::Lerp(0, MaxSpeed, accumulatedAcceleration / accelerationTime);
        }

        // 确保速度在合理范围内
        CurrentSpeed = FMath::Clamp(CurrentSpeed, 0.0f, MaxSpeed);
        OurMovementComponent->AddInputVector(GetActorForwardVector() * CurrentSpeed);
    }
}
//检测出界
void ABasePawn::IsItOutOfBounds()
{
    FVector NowLocation = GetActorLocation();
    /*if(NowLocation.X != FMath::Clamp(NowLocation.X, -30000.f, 30000.f)
        || NowLocation.Y != FMath::Clamp(NowLocation.Y, -30000.f, 30000.f)
        || NowLocation.Z != FMath::Clamp(NowLocation.Z, -10000.f, 20000.f))
    {
        if(Type==TargetType::PLANE)
            this->SetActorLocation(FVector(FMath::RandRange(-20000, 20000), FMath::RandRange(-20000, 20000), FMath::RandRange(2500, 4000)));
        else
            this->SetActorLocation(FVector(FMath::RandRange(-20000, 20000), FMath::RandRange(-20000, 20000), FMath::RandRange(0, 200)));
    }*/
}
