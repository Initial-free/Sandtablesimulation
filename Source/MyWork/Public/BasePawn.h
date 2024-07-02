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
    void InitBaseValue();            //���û�����ֵ
    void InitCameraComponent();      //��ʼ������ͷ
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
	//���û���������
	//��ǰ����
	UPROPERTY(VisibleAnywhere, Category = "BasicalMessage", meta = (AllowPrivateAccess = "true"))
	FVector CurrentLocation;
    //��ǰ����
    UPROPERTY(VisibleAnywhere, Category = "BasicalMessage", meta = (AllowPrivateAccess = "true"))
    FVector EndLocation;

    //��ʼ��ת��λ��
    UPROPERTY(VisibleAnywhere, Category = "BasicalMessage", meta = (AllowPrivateAccess = "true"))
    FRotator InitialRotation;

    UPROPERTY(VisibleAnywhere, Category = "BaseMesh", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* VehicleMesh;
    UPROPERTY()
    class UParticleSystemComponent* OurParticleSystem;

    //�ƶ����
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
   

	//�����������
    //���������������
    UFUNCTION(BlueprintCallable, Category = "TheCamera")
    void VehicleChangeCamera();
    // ���ɱ�����
    UFUNCTION(BlueprintCallable, Category = "TheCamera")
    void CameraSpringArmZoomIn();
    UFUNCTION(BlueprintCallable, Category = "TheCamera")
    void CameraSpringArmZoomOut();
    // �����ת��
    UFUNCTION(BlueprintCallable, Category = "TheCamera")
    void CameraTurnAround(float AxisValue);
    // ����� LookUp
    UFUNCTION(BlueprintCallable, Category = "TheCamera")
    void CameraLookUp(float AxisValue);
    UFUNCTION(BlueprintCallable, Category = "TheCamera")
    void CameraFollow();


    UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArm;
    //  �ⲿ�����
    UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* ExternalCamera;

    //  �ڲ������
    UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* InternalCamera;
    UPROPERTY(VisibleAnywhere)
    bool CameraType;
    // ������ת�Ƕ�
    UPROPERTY(VisibleAnywhere)
    int64 BaseTurnRate;
    // ���������Ƕ�
    UPROPERTY(VisibleAnywhere)
    float BaseLookUpRate;
    UPROPERTY(VisibleAnywhere)
    float LookUpInput;
    //����Ƿ����
    UPROPERTY(VisibleAnywhere)
    bool CameraFllow;

    //ƫ�ƽǶ�
    FRotator SavedRelativeRotation;
public:
    UPROPERTY(VisibleAnywhere)
    float accumulatedAcceleration = 0;

    UFUNCTION()
    float GetDecelerationTime(float currentSpeed) const;
    
    UPROPERTY(VisibleAnywhere)
    float accelerationTime = 1.0f; //���ٳ���ʱ��
    
    UPROPERTY(VisibleAnywhere)
    float CurrentSpeed = 1.0f;
    
    UPROPERTY(VisibleAnywhere)
    float MaxSpeed =8.0f; // ����ٶ�Ϊ100��λ/��

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
    float AccelerationRate = 2.0f; // ÿ�����ӵ��ٶ�
public:
    void MakeFire();
    // ǹ������������λ�õ�ƫ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    FVector MuzzleOffset;
    // Ҫ���ɵķ�������
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class AWeapon> ProjectileClass;

    UPROPERTY(EditAnywhere)
    FTimerHandle ForWardHandle;
    UPROPERTY(EditAnywhere)
    FTimerHandle TurnHandle;

public:
    //��в�뾶
    double Radius;
    //��ų�����ǿ��
    double EmInterference1, EmInterference2;
    //����
    TargetType Type;
    //��ս״̬
    CombatStatus Status;
    UTextRenderComponent* TheTextInstance;

    ATheText* TheText;
};
