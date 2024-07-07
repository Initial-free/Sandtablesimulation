// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MYWORK_API TemplateClass
{
public:
	TemplateClass();
	~TemplateClass();
public:
	template<typename T>
	void SetArrayVariable(T ThePawn);
};

template<typename T>
inline void TemplateClass::SetArrayVariable(T ThePawn)
{
    ShowPartcile(ThePawn);
    if (ThePawn->ForWardHandle.IsValid())
        GetWorldTimerManager().ClearTimer(ThePawn->ForWardHandle);
    if (ThePawn->TurnHandle.IsValid())
        GetWorldTimerManager().ClearTimer(ThePawn->TurnHandle);
    if (ThePawn->OutHandle.IsValid())
        GetWorldTimerManager().ClearTimer(ThePawn->OutHandle);


    int32 Index = EnemyG.IndexOfByKey(ThePawn);
    ThePawn->ToDeath();
    if (Index != INDEX_NONE)
    {
        EnemyG.RemoveAt(Index);
    }
}
