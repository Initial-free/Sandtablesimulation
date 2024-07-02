// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMovementComponent.h"

void UBaseMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 确保所有事物持续有效，以便进行移动。
    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
    {
        return;
    }

    // 获取然后清除pawn类中设置的移动向量
    //ConsumeInputVector()处理玩家的输入
    //FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 150.0f;
    FVector DesiredMovementThisFrame = ConsumeInputVector() * DeltaTime * 150.0f;
    if (!DesiredMovementThisFrame.IsNearlyZero())
    {
        FHitResult Hit;
        SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

        // 若发生碰撞，尝试滑过去
        if (Hit.IsValidBlockingHit())
        {
            //SlideAlongSurface 会处理沿墙壁和斜坡等碰撞表面平滑滑动所涉及的计算和物
            //而非直接停留原地，粘在墙壁或斜坡上。
            SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
        }
    }
}

