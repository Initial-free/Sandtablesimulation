// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

AWeapon::AWeapon()
{
 	PrimaryActorTick.bCanEverTick = false;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
    // 将球体的碰撞配置文件名称设置为"Projectile"。
   
    // 用球体进行简单的碰撞展示。
    //CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    //CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
    //CollisionComponent->OnComponentHit.AddDynamic(this, &AWeapon::OnHit);
    // 设置球体的碰撞半径。
    //CollisionComponent->InitSphereRadius(0.0f);
    // 将根组件设置为碰撞组件。
    //RootComponent = CollisionComponent;
    // 使用此组件驱动发射物的移动。
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
    ProjectileMovementComponent->InitialSpeed = 3000.0f;
    ProjectileMovementComponent->MaxSpeed = 3000.0f;
   ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = true;
    ProjectileMovementComponent->Bounciness = 0.0f;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;


    ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("StaticMesh'/Game/VehicleAsset/Mesh/bullet/bullet.bullet'"));
    ProjectileMeshComponent->SetupAttachment(RootComponent);
    if (Mesh.Succeeded())
    {
        ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
        ProjectileMeshComponent->SetRelativeRotation(FRotator(0.0f,90.0f,0.0f));
        ProjectileMeshComponent->SetRelativeLocation(FVector(100.0f,0.0f,0.0f));
        ProjectileMeshComponent->SetWorldScale3D(FVector(0.5f));
    }
    ProjectileMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    InitialLifeSpan = 3.0f;
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
    Destroy();
}

