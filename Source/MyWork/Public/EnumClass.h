// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
enum class TargetType
{
	PLANE = 1,
	TANK,
	ARMORED_CAR
	
};
enum class CombatStatus 
{ 
	STATUS_1=1, 
	STATUS_2, 
	STATUS_3 
};

static int GetTargetPriority(const TargetType& a)
{
    if (a == TargetType::PLANE)
        return 1;
    else if (a == TargetType::TANK)
        return 2;
    else
        return 3;
}
static int GetStatusPriority(const CombatStatus& a)
{
    if (a == CombatStatus::STATUS_1)
        return 1;
    else if (a == CombatStatus::STATUS_2)
        return 2;
    else
        return 3;
}