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
    // ����Ĭ�����
    //AutoPossessPlayer = EAutoReceiveInput::Player0;
    // ���������Ϊ������Ӧ������
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
    //������Ļ�������
    BaseTurnRate = 5;
    BaseLookUpRate = 1.0f;
    LookUpInput = 0.0f;
    CameraType = true;
    CameraFllow = true;
    //��ʼλ��
    InitialRotation = GetActorRotation();
    Threatlevel = 0;
    //��ʼ����в�̶�
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

    // �������λ��
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

    // ��������������ӵ����ɱ�
    ExternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ExternalCamera"));
    ExternalCamera->FieldOfView = 90.0f;
    ExternalCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    OurMovementComponent = CreateDefaultSubobject<UBaseMovementComponent>(TEXT("CustomMovementComponent"));
    OurMovementComponent->UpdatedComponent = RootComponent;
    //���ó��������λ��
    InternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InternalCamera"));
    // ����λ�ã���ͬ���ͳ� ��ͼ����λ��
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
//���а�����
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
//��ȡǰ��ģ��
UPawnMovementComponent* ABasePawn::GetMovementComponent() const
{
    return OurMovementComponent;
}
//�ı�ݳ��ӽ�
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
//����۵�����
void ABasePawn::CameraSpringArmZoomIn()
{
    if (SpringArm->TargetArmLength > 300.0f)
    {
        SpringArm->TargetArmLength = SpringArm->TargetArmLength - 20.0f;
    }
}

void ABasePawn::CameraSpringArmZoomOut()
{
    //������ɱ�С��������
    if (SpringArm->TargetArmLength < 1000.f)
    {
        SpringArm->TargetArmLength = SpringArm->TargetArmLength + 20.0f;
    }
}
//����ͷ������ת
void ABasePawn::CameraTurnAround(float AxisValue)
{
    if(!CameraFllow)
    {
        //���������������򵥶���ת
        float CameraYaw = AxisValue;
        if (AxisValue)
            CameraYaw = AxisValue * BaseTurnRate;
        FRotator CameraRotator = FRotator(0.0f, CameraYaw, 0.0f);
        InternalCamera->AddRelativeRotation(CameraRotator);
        SpringArm->AddRelativeRotation(CameraRotator);
    }
}
//ת��
void ABasePawn::Turn(float AxisValue)
{
    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += AxisValue;
    if (CameraFllow)
    {
        //��������������ȫ����ת
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
//����ͷ̧��
void ABasePawn::CameraLookUp(float AxisValue)
{
    
    //LookUpInput = axis_value * BaseLookUpRate * GetWorld()->DeltaTimeSeconds;
    LookUpInput = AxisValue * BaseLookUpRate;
   
    // Pitch ��ת����
    FRotator spring_arm_rotator = SpringArm->GetRelativeRotation();
    float spring_arm_pitch = spring_arm_rotator.Pitch;
    // �����ǰ�����Ƕ�С��-66.6��������ֵ�Ǹ��ģ���ִ����ת
    if (!(spring_arm_pitch < -66.6f && LookUpInput < 0.0f))
    {
        // �����ǰ�����Ƕȴ���0��������ֵ�����ģ���ִ����ת
        if (!(spring_arm_pitch > 0.0f && LookUpInput > 0.0f))
        {
            //���������ת
            FRotator CameraRotator = FRotator(LookUpInput, 0.0f, 0.0f);
            InternalCamera->AddRelativeRotation(CameraRotator);
            SpringArm->AddRelativeRotation(CameraRotator);
        }
    }
}
//����ͷ����
void ABasePawn::CameraFollow()
{
    CameraFllow = !CameraFllow;
    if (CameraFllow)
    {
        //�����ʱ���棬��������������ת����
        FRotator NewRotation = GetActorRotation();
       
        SpringArm->SetRelativeRotation(NewRotation);
        InternalCamera->SetRelativeRotation(NewRotation);
        SpringArm->SetWorldRotation(NewRotation);
        InternalCamera->SetWorldRotation(NewRotation);
    }
}
//��ǰ�ƶ�
void ABasePawn::MoveForward(float AxisValue)
{
    if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
    {
        if (AxisValue != 0) // ��������
        {
            // ����
            accumulatedAcceleration += FMath::Abs(AxisValue) * GetWorld()->GetDeltaSeconds() * AccelerationRate;
            accumulatedAcceleration = FMath::Clamp(accumulatedAcceleration, 0.0f, accelerationTime);
            CurrentSpeed = FMath::Lerp(0, MaxSpeed, accumulatedAcceleration / accelerationTime);
        }
        else
        {
            // ����
            accumulatedAcceleration -= GetWorld()->GetDeltaSeconds();
            accumulatedAcceleration = FMath::Clamp(accumulatedAcceleration, 0.0f, accelerationTime);
            CurrentSpeed = FMath::Lerp(0, MaxSpeed, accumulatedAcceleration / accelerationTime);
        }

        // ȷ���ٶ��ں���Χ��
        CurrentSpeed = FMath::Clamp(CurrentSpeed, 0.0f, MaxSpeed);

        OurMovementComponent->AddInputVector(GetActorForwardVector()* CurrentSpeed);
    }
}
//���ת��
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
        //��������������ȫ����ת
        // ��ȡ��ǰ��ת
        FRotator CurrentRotation = GetActorRotation();

        // �����ֵ�ٶȣ����Ը�����Ҫ����
        float InterpSpeed = 10.0f;

        // �ڵ�ǰ��ת��Ŀ����ת֮���ֵ
        FRotator InterpolatedRotation = FMath::RInterpTo(CurrentRotation, NewRotation, GetWorld()->GetDeltaSeconds(), InterpSpeed);
        if (InterpolatedRotation == NewRotation)
            isTure = false;
        // ���ò�ֵ�����ת
        SetActorRotation(InterpolatedRotation);
    }
    else
    {
        if (OldAxisValue!=0)
        {
            // ��ȡ��ǰ��ת
            FRotator CurrentRotation = GetActorRotation();

            // �����ֵ�ٶȣ����Ը�����Ҫ����
            float InterpSpeed = 10.0f;

            // �ڵ�ǰ��ת��Ŀ����ת֮���ֵ
            FRotator InterpolatedRotation = FMath::RInterpTo(CurrentRotation, NewRotation, GetWorld()->GetDeltaSeconds(), InterpSpeed);

            // ���ò�ֵ�����ת
            SetActorRotation(InterpolatedRotation);
        }
    }
}
//�����ǰ�ƶ�
void ABasePawn::RandomMoveForward()
{
    if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
    {
        accumulatedAcceleration += FMath::Abs(1) * GetWorld()->GetDeltaSeconds() * AccelerationRate;
        accumulatedAcceleration = FMath::Clamp(accumulatedAcceleration, 0.0f, accelerationTime);
        CurrentSpeed = FMath::Lerp(0, MaxSpeed, accumulatedAcceleration / accelerationTime);
        CurrentSpeed = FMath::RandRange(CurrentSpeed-4.0f, CurrentSpeed+4.0f);

        // ȷ���ٶ��ں���Χ��
        CurrentSpeed = FMath::Clamp(CurrentSpeed, 0.0f, MaxSpeed);
        OurMovementComponent->AddInputVector(GetActorForwardVector() * CurrentSpeed);
    }
}
//����ʱ��
float ABasePawn::GetDecelerationTime(float currentSpeed) const
{
    float timeConstant = 1.0f / (CurrentSpeed + 1.0f); // ʱ�䳣�����ٶ�Խ�ߣ�ʱ��Խ��
    return timeConstant;
}
void ABasePawn::GunFire()
{
    // ��ͼ���䷢����
    if (ProjectileClass)
    {
        // ��ȡ������任��
        FVector CameraLocation;
        FRotator CameraRotation;
        GetActorEyesViewPoint(CameraLocation, CameraRotation);

        // ����MuzzleOffset�����Կ��������ǰ���ɷ����
        MuzzleOffset.Set(200.0f, 0.0f, 0.0f);

        // ��MuzzleOffset��������ռ�任������ռ䡣
        FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

        // ʹĿ�귽����������б��
        FRotator MuzzleRotation = CameraRotation;
        MuzzleRotation.Pitch += EnemyPitch;

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
//���ø߶�����
void ABasePawn::SetEnemyPitch(float Temp)
{
    EnemyPitch = Temp;
}
//����״̬
void ABasePawn::InitialPosition()
{
    SetActorRotation(InitialRotation);
}
//��������
void ABasePawn::ToDeath()
{
    this->TheText->Destroy();
    this->Destroy();
}

//ɲ��ϵͳ
void ABasePawn::HandBrake(float AxisValue)
{
    if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
    {
        if(AxisValue)
        {
            // ����
            accumulatedAcceleration -= GetWorld()->GetDeltaSeconds()*5;
            accumulatedAcceleration = FMath::Clamp(accumulatedAcceleration, 0.0f, accelerationTime);
            CurrentSpeed = FMath::Lerp(0, MaxSpeed, accumulatedAcceleration / accelerationTime);
        }

        // ȷ���ٶ��ں���Χ��
        CurrentSpeed = FMath::Clamp(CurrentSpeed, 0.0f, MaxSpeed);
        OurMovementComponent->AddInputVector(GetActorForwardVector() * CurrentSpeed);
    }
}
//������
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
