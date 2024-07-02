// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMovementComponent.h"

void UBaseMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ȷ���������������Ч���Ա�����ƶ���
    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
    {
        return;
    }

    // ��ȡȻ�����pawn�������õ��ƶ�����
    //ConsumeInputVector()������ҵ�����
    //FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 150.0f;
    FVector DesiredMovementThisFrame = ConsumeInputVector() * DeltaTime * 150.0f;
    if (!DesiredMovementThisFrame.IsNearlyZero())
    {
        FHitResult Hit;
        SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

        // ��������ײ�����Ի���ȥ
        if (Hit.IsValidBlockingHit())
        {
            //SlideAlongSurface �ᴦ����ǽ�ں�б�µ���ײ����ƽ���������漰�ļ������
            //����ֱ��ͣ��ԭ�أ�ճ��ǽ�ڻ�б���ϡ�
            SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
        }
    }
}

