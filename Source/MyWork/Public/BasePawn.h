#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <GameFramework/SpringArmComponent.h>
#include "Components/TextRenderComponent.h"
#include "BaseMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Weapon.h"
#include "EnumClass.h"
#include "TheText.h"
#include "BasePawn.generated.h"
UCLASS()
class MYWORK_API ABasePawn : public APawn
{   
    GENERATED_BODY()

public:
    ABasePawn();
    void InitBaseValue();            //设置基本数值
    void InitCameraComponent();      //初始化摄像头
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual UPawnMovementComponent* GetMovementComponent() const override;
public:
	//设置基本的数据
	//当前坐标
	UPROPERTY(VisibleAnywhere, Category = "BasicalMessage", meta = (AllowPrivateAccess = "true"))
	FVector CurrentLocation;
    //当前坐标
    UPROPERTY(VisibleAnywhere, Category = "BasicalMessage", meta = (AllowPrivateAccess = "true"))
    FVector EndLocation;

    //初始旋转体位置
    UPROPERTY(VisibleAnywhere, Category = "BasicalMessage", meta = (AllowPrivateAccess = "true"))
    FRotator InitialRotation;

    UPROPERTY(VisibleAnywhere, Category = "BaseMesh", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* VehicleMesh;
    UPROPERTY()
    class UParticleSystemComponent* OurParticleSystem;

    //移动组件
    UPROPERTY()
    class UBaseMovementComponent* OurMovementComponent;
    UFUNCTION(BlueprintCallable, Category = "TheCamera")
    void Turn(float AxisValue);
    UFUNCTION()
    void MoveForward(float AxisValue);

    UFUNCTION()
    void RandomTurn();
    UFUNCTION()
    void RandomMoveForward();

    UFUNCTION()
    void HandBrake(float AxisValue);
    UFUNCTION()
    void ParticleToggle();
    UFUNCTION()
    void InitialPosition();
   
public:
   

	//摄像机的设置
    //更改摄像机的配置
    UFUNCTION(BlueprintCallable, Category = "TheCamera")
    void VehicleChangeCamera();
    // 弹簧臂缩放
    UFUNCTION(BlueprintCallable, Category = "TheCamera")
    void CameraSpringArmZoomIn();
    UFUNCTION(BlueprintCallable, Category = "TheCamera")
    void CameraSpringArmZoomOut();
    // 摄像机转向
    UFUNCTION(BlueprintCallable, Category = "TheCamera")
    void CameraTurnAround(float AxisValue);
    // 摄像机 LookUp
    UFUNCTION(BlueprintCallable, Category = "TheCamera")
    void CameraLookUp(float AxisValue);
    UFUNCTION(BlueprintCallable, Category = "TheCamera")
    void CameraFollow();


    UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArm;
    //  外部摄像机
    UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* ExternalCamera;

    //  内部摄像机
    UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* InternalCamera;
    UPROPERTY(VisibleAnywhere)
    bool CameraType;
    // 基础旋转角度
    UPROPERTY(VisibleAnywhere)
    int64 BaseTurnRate;
    // 基础看望角度
    UPROPERTY(VisibleAnywhere)
    float BaseLookUpRate;
    UPROPERTY(VisibleAnywhere)
    float LookUpInput;
    //相机是否跟随
    UPROPERTY(VisibleAnywhere)
    bool CameraFllow;

    //偏移角度
    FRotator SavedRelativeRotation;
public:
    UPROPERTY(VisibleAnywhere)
    float accumulatedAcceleration = 0;

    UFUNCTION()
    float GetDecelerationTime(float currentSpeed) const;
    
    UPROPERTY(VisibleAnywhere)
    float accelerationTime = 1.0f; //加速持续时间
    
    UPROPERTY(VisibleAnywhere)
    float CurrentSpeed = 1.0f;
    
    UPROPERTY(VisibleAnywhere)
    float MaxSpeed =8.0f; // 最大速度为100单位/秒

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
    float AccelerationRate = 2.0f; // 每秒增加的速度
public:
    void MakeFire();
    // 枪口相对于摄像机位置的偏移
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    FVector MuzzleOffset;
    // 要生成的发射物类
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class AWeapon> ProjectileClass;

    UPROPERTY(EditAnywhere)
    FTimerHandle ForWardHandle;
    UPROPERTY(EditAnywhere)
    FTimerHandle TurnHandle;

public:
    //威胁半径
    double Radius;
    //电磁场干扰强度
    double EmInterference1, EmInterference2;
    //类型
    TargetType Type;
    //作战状态
    CombatStatus Status;
    UTextRenderComponent* TheTextInstance;

    ATheText* TheText;
};
