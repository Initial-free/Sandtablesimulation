// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyWorkGameModeBase.h"
#include "TimerManager.h"
#include "KMeans.h"
AMyWorkGameModeBase::AMyWorkGameModeBase()
{
    PrimaryActorTick.bCanEverTick = true;
    Totalnum = 50;
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
}

void AMyWorkGameModeBase::Initialization()
{
    {
        FName InstanceName = FName(*FString::Printf(TEXT("Mine")));
        MyPawn = GetWorld()->SpawnActor<AVehiclePawn>(AVehiclePawn::StaticClass());
        while (MyPawn == NULL)
            MyPawn = GetWorld()->SpawnActor<AVehiclePawn>(AVehiclePawn::StaticClass());
    }
    for (int32 i = 0; (EnemyG.Num())< Totalnum; ++i) 
    {
        //获取随机数选择生成
       
        int32 Choose = FMath::Clamp(FMath::RandRange(0, 100), 30, 60);
        if (Choose == 30)
        { 
            FName InstanceName = FName(*FString::Printf(TEXT("TheTankEnemy_%d"), i));
            ABasePawn* mySphere = GetWorld()->SpawnActor<AVehiclePawn>(AVehiclePawn::StaticClass());
            if (mySphere)
            {
                mySphere->Tags.Add(InstanceName);
                EnemyG.Add(mySphere);
            }
        }
        else if(Choose == 60)
        {
            FName InstanceName = FName(*FString::Printf(TEXT("TheFighterEnemy_%d"), i));
            ABasePawn* mySphere = GetWorld()->SpawnActor<AFighterPawn>(AFighterPawn::StaticClass());
            if (mySphere)
            {
                mySphere->Tags.Add(InstanceName);
                EnemyG.Add(mySphere);
            }
        }
        else
        {
            FName InstanceName = FName(*FString::Printf(TEXT("TheArmoredCarEnemy_%d"), i));
            ABasePawn* mySphere = GetWorld()->SpawnActor<AArmoredCarPawn>(AArmoredCarPawn::StaticClass());
            if (mySphere)
            {
                mySphere->Tags.Add(InstanceName);
                EnemyG.Add(mySphere);
            }
        }

    }
}

void AMyWorkGameModeBase::TheControl()
{
    static long mmm = 1000;
    mmm--;
    if (mmm < 0)
    {
        for (ABasePawn* ThePawn : EnemyG)
        {
            if (ThePawn->IsA<AVehiclePawn>())
            {
                if(ThePawn->ForWardHandle.IsValid())
                    GetWorldTimerManager().ClearTimer(ThePawn->ForWardHandle);
                if (ThePawn->TurnHandle.IsValid())
                    GetWorldTimerManager().ClearTimer(ThePawn->TurnHandle);
            }
            else if (ThePawn->IsA<AFighterPawn>())
            {
                if (ThePawn->ForWardHandle.IsValid())
                    GetWorldTimerManager().ClearTimer(ThePawn->ForWardHandle);
                if (ThePawn->TurnHandle.IsValid())
                    GetWorldTimerManager().ClearTimer(ThePawn->TurnHandle);
            }
            else if (ThePawn->IsA<AArmoredCarPawn>())
            {
                if (ThePawn->ForWardHandle.IsValid())
                    GetWorldTimerManager().ClearTimer(ThePawn->ForWardHandle);
                if (ThePawn->TurnHandle.IsValid())
                    GetWorldTimerManager().ClearTimer(ThePawn->TurnHandle);
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
        }
        else if (ThePawn->IsA<AFighterPawn>())
        {
           GetWorldTimerManager().SetTimer(ThePawn->ForWardHandle, ThePawn, &AFighterPawn::RandomMoveForward,.01f, true, .0f);
           GetWorldTimerManager().SetTimer(ThePawn->TurnHandle, ThePawn, &AFighterPawn::RandomTurn, .5f, true, .0f);
        }
        else if (ThePawn->IsA<AArmoredCarPawn>())
        {
            GetWorldTimerManager().SetTimer(ThePawn->ForWardHandle, ThePawn, &AArmoredCarPawn::RandomMoveForward, .01f, true, .0f);
            GetWorldTimerManager().SetTimer(ThePawn->TurnHandle, ThePawn, &AArmoredCarPawn::RandomTurn, .5f, true, .0f);
        }
    }

}

void AMyWorkGameModeBase::TheCalculation()
{
    KMeans kmeans(15,1000,1e-5);
    kmeans.SetBasicdata(EnemyG, MyPawn);
    kmeans.Run();
}


