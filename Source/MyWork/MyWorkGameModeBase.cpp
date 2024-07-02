// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyWorkGameModeBase.h"
#include "TimerManager.h"
#include "KMeans.h"
#include "TemplateClass.h"
#include <Actions/PawnAction.h>
#include "TheController.h"
AMyWorkGameModeBase::AMyWorkGameModeBase()
{
    PrimaryActorTick.bCanEverTick = true;
    Totalnum =100;
}


void AMyWorkGameModeBase::BeginPlay()
{
    Initialization();
    SetTimer();
}

void AMyWorkGameModeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TheControl();
    TheCalculation();
    ShowText();
}

void AMyWorkGameModeBase::Initialization()
{
    InitialBaseArray();
    InitialLocationAndRotation();
   
}
void AMyWorkGameModeBase::InitialBaseArray()
{
    {
        FName InstanceName = FName(*FString::Printf(TEXT("Mine")));
        MyPawn = GetWorld()->SpawnActor<AVehiclePawn>(AVehiclePawn::StaticClass());
        while (MyPawn == NULL)
            MyPawn = GetWorld()->SpawnActor<AVehiclePawn>(AVehiclePawn::StaticClass());
        MyPawn->SetActorLocation(FVector(FMath::RandRange(-5000, 5000), FMath::RandRange(-5000, 5000), FMath::RandRange(300, 500)));
        GetWorldTimerManager().SetTimer(MyPawn->DeathHandle, MyPawn, &AVehiclePawn::GunFire, 1.f, true, .0f);
    }
    for (int32 i = 0; (EnemyG.Num()) < Totalnum; ++i)
    {
        //获取随机数选择生成
        {
            FName InstanceName = FName(*FString::Printf(TEXT("TheTankEnemy_%d"), i));
            ABasePawn* mySphere = GetWorld()->SpawnActor<AVehiclePawn>(AVehiclePawn::StaticClass());
            if (mySphere)
            {
                mySphere->SetActorLocation(FVector(FMath::RandRange(-5000, 5000), FMath::RandRange(-5000, 5000), FMath::RandRange(300, 500)));
                mySphere->Tags.Add(InstanceName);
                EnemyG.Add(mySphere);

            }
        }

        {
            FName InstanceName = FName(*FString::Printf(TEXT("TheFighterEnemy_%d"), i));
            ABasePawn* mySphere = GetWorld()->SpawnActor<AFighterPawn>(AFighterPawn::StaticClass());
            if (mySphere)
            {
                mySphere->SetActorLocation(FVector(FMath::RandRange(-5000, 5000), FMath::RandRange(-5000, 5000), FMath::RandRange(300, 1000)));
                mySphere->Tags.Add(InstanceName);
                EnemyG.Add(mySphere);
            }
        }

        {
            FName InstanceName = FName(*FString::Printf(TEXT("TheArmoredCarEnemy_%d"), i));
            ABasePawn* mySphere = GetWorld()->SpawnActor<AArmoredCarPawn>(AArmoredCarPawn::StaticClass());
            if (mySphere)
            {
                mySphere->SetActorLocation(FVector(FMath::RandRange(-5000, 5000), FMath::RandRange(-5000, 5000), FMath::RandRange(300, 500)));
                mySphere->Tags.Add(InstanceName);
                EnemyG.Add(mySphere);
            }
        }
    }
}
void AMyWorkGameModeBase::InitialLocationAndRotation()
{
    for (ABasePawn* ThePawn : EnemyG)
    {
        if (ThePawn->IsA<AVehiclePawn>())
        {
            ThePawn->SetActorLocation(FVector(FMath::RandRange(-5000, 5000), FMath::RandRange(-5000, 5000), FMath::RandRange(0, 500)));
        }
        else if (ThePawn->IsA<AFighterPawn>())
        {
            ThePawn->SetActorLocation(FVector(FMath::RandRange(-5000, 5000), FMath::RandRange(-5000, 5000), FMath::RandRange(500, 3000)));
        }
        else if (ThePawn->IsA<AArmoredCarPawn>())
        {
            ThePawn->SetActorLocation(FVector(FMath::RandRange(-5000, 5000), FMath::RandRange(-5000, 5000), FMath::RandRange(0, 500)));
        }
        ThePawn->SetActorRotation(FRotator(0.0f, FMath::RandRange(-180.f,180.f),0.0f));
    }
    
}

void AMyWorkGameModeBase::AttackControl()
{
    
    int num = EnemyG.Num();
    if (num == 0)
        return;
    
    if (num != 0 && MyPawn && EnemyG[num - 1])
    {
        EnemyG.HeapSort([](const ABasePawn& a, const ABasePawn& b) {
        return a.Threatlevel < b.Threatlevel;
        });
        
        // 获取Pawn和目标Actor之间的方向向量
        FVector Direction = EnemyG[num-1]->GetActorLocation() - MyPawn->GetActorLocation();
        Direction.Normalize();

        // 获取Pawn的当前朝向
        FRotator CurrentRotation = MyPawn->GetControlRotation();

        // 计算目标旋转
        FRotator TargetRotation = Direction.Rotation();

        // 计算Yaw和Pitch的差异
        float YawDifference = TargetRotation.Yaw - CurrentRotation.Yaw;
        float PitchDifference = TargetRotation.Pitch - CurrentRotation.Pitch;

        // 确保Yaw差异在-180到180度之间
        if (YawDifference > 180.0f)
        {
            YawDifference -= 360.0f;
        }
        else if (YawDifference < -180.0f)
        {
            YawDifference += 360.0f;
        }
        for (int i = 0; i < YawDifference*5; i++)
        {
            MyPawn->Turn(1);
        }
        MyPawn->SetEnemyPitch(TargetRotation.Pitch);
    }
}

void AMyWorkGameModeBase::TheControl()
{
    for (ABasePawn* ThePawn : EnemyG)
    {
        if (ThePawn)
        {
            if (ThePawn->IsA<AVehiclePawn>() && ThePawn->IsDeath)
            {
                if (ThePawn->ForWardHandle.IsValid())
                    GetWorldTimerManager().ClearTimer(ThePawn->ForWardHandle);
                if (ThePawn->TurnHandle.IsValid())
                    GetWorldTimerManager().ClearTimer(ThePawn->TurnHandle);
                if (ThePawn->OutHandle.IsValid())
                    GetWorldTimerManager().ClearTimer(ThePawn->OutHandle);
                int32 Index = EnemyG.IndexOfByKey(ThePawn);
                ThePawn->TheText->Destroy();
                ThePawn->Destroy();
                if (Index != INDEX_NONE)
                {
                    EnemyG.RemoveAt(Index);
                }
                break;
            }
            else if (ThePawn->IsA<AFighterPawn>() && ThePawn->IsDeath)
            {
                if (ThePawn->ForWardHandle.IsValid())
                    GetWorldTimerManager().ClearTimer(ThePawn->ForWardHandle);
                if (ThePawn->TurnHandle.IsValid())
                    GetWorldTimerManager().ClearTimer(ThePawn->TurnHandle);
                if (ThePawn->OutHandle.IsValid())
                    GetWorldTimerManager().ClearTimer(ThePawn->OutHandle);
                int32 Index = EnemyG.IndexOfByKey(ThePawn);
                ThePawn->TheText->Destroy();
                ThePawn->Destroy();
                if (Index != INDEX_NONE)
                {
                    EnemyG.RemoveAt(Index);
                }
                break;
            }
            else if (ThePawn->IsA<AArmoredCarPawn>() && ThePawn->IsDeath)
            {
                if (ThePawn->ForWardHandle.IsValid())
                    GetWorldTimerManager().ClearTimer(ThePawn->ForWardHandle);
                if (ThePawn->TurnHandle.IsValid())
                    GetWorldTimerManager().ClearTimer(ThePawn->TurnHandle);
                if (ThePawn->OutHandle.IsValid())
                    GetWorldTimerManager().ClearTimer(ThePawn->OutHandle);
                ThePawn->TheText->Destroy();
                ThePawn->Destroy();
                int32 Index = EnemyG.IndexOfByKey(ThePawn);
                if (Index != INDEX_NONE)
                {
                    EnemyG.RemoveAt(Index);
                }
                break;
            }
        }
    }
}

void AMyWorkGameModeBase::SetTimer()
{
    for (ABasePawn* ThePawn : EnemyG)
    {
        if (ThePawn->IsA<AVehiclePawn>())
        {
            GetWorldTimerManager().SetTimer(ThePawn->ForWardHandle,ThePawn,&AVehiclePawn::RandomMoveForward,.1f,true,.0f);
            GetWorldTimerManager().SetTimer(ThePawn->TurnHandle, ThePawn, &AVehiclePawn::RandomTurn, .2f, true, .0f);
            GetWorldTimerManager().SetTimer(ThePawn->OutHandle, ThePawn, &AVehiclePawn::IsItOutOfBounds, 1.f, true, .0f);
        }
        else if (ThePawn->IsA<AFighterPawn>())
        {
           GetWorldTimerManager().SetTimer(ThePawn->ForWardHandle, ThePawn, &AFighterPawn::RandomMoveForward,.01f, true, .0f);
           GetWorldTimerManager().SetTimer(ThePawn->TurnHandle, ThePawn, &AFighterPawn::RandomTurn, .5f, true, .0f);
           GetWorldTimerManager().SetTimer(ThePawn->OutHandle, ThePawn, &AVehiclePawn::IsItOutOfBounds, 1.f, true, .0f);
        }
        else if (ThePawn->IsA<AArmoredCarPawn>())
        {
            GetWorldTimerManager().SetTimer(ThePawn->ForWardHandle, ThePawn, &AArmoredCarPawn::RandomMoveForward, .1f, true, .0f);
            GetWorldTimerManager().SetTimer(ThePawn->TurnHandle, ThePawn, &AArmoredCarPawn::RandomTurn, .2f, true, .0f);
            GetWorldTimerManager().SetTimer(ThePawn->OutHandle, ThePawn, &AVehiclePawn::IsItOutOfBounds,1.f, true, .0f);
        }
    }

}

void AMyWorkGameModeBase::TheCalculation()
{
    KMeans kmeans(15,1000,1e-4);
    kmeans.SetBasicdata(EnemyG, MyPawn);
    kmeans.Run();
    kmeans.GetAllData(EnemyNameMap);
}

void AMyWorkGameModeBase::ShowText()
{
    for (ABasePawn* ThePawn : EnemyG)
    {
        if (ThePawn&&ThePawn->TheText)
        {
            int* index = EnemyNameMap.Find(FName(*ThePawn->GetName()));
            float threat = ThePawn->Threatlevel;
            ThePawn->TheText->SetThretLvel(*index+1);
            ThePawn->TheText->AddText(threat);
        }
    }
}


