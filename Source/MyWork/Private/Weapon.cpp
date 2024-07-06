// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "BasePawn.h"
AWeapon::AWeapon()
{
 	PrimaryActorTick.bCanEverTick = false;
    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
    }
    if (!CollisionComponent)
    {
        // ��������м򵥵���ײչʾ��
        CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
        // �����������ײ�뾶��
        CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

        CollisionComponent->OnComponentHit.AddDynamic(this, &AWeapon::OnHit);
        
        CollisionComponent->InitSphereRadius(30.0f);
        // �����������Ϊ��ײ�����
        RootComponent = CollisionComponent;
    }
    if (!ProjectileMovementComponent)
    {
        // ʹ�ô����������������ƶ���
        ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
        ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
        ProjectileMovementComponent->InitialSpeed = 3000.0f;
        ProjectileMovementComponent->MaxSpeed = 3000.0f;
        ProjectileMovementComponent->bRotationFollowsVelocity = true;
        ProjectileMovementComponent->bShouldBounce = true;
        ProjectileMovementComponent->Bounciness = 0.3f;
        ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
    }


    ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/bullet/bullet.bullet'"));
    ProjectileMeshComponent->SetupAttachment(RootComponent);
    if (Mesh.Succeeded())
    {
        ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
        ProjectileMeshComponent->SetRelativeRotation(FRotator(0.0f,90.0f,0.0f));
        ProjectileMeshComponent->SetRelativeLocation(FVector(100.0f,0.0f,0.0f));
        ProjectileMeshComponent->SetWorldScale3D(FVector(0.8f));
    }
    InitialLifeSpan = 6.0f;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AWeapon::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
    {
        OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
    }
    // ���ٱ�����������
    if (OtherActor && OtherActor != this)
    {
        ABasePawn* MyActor = Cast<ABasePawn>(OtherActor);

        // ���ת���ɹ�
        if (MyActor)
        {
            MyActor->IsDeath = true;
        }
    }
    Destroy();
}

