// Fill out your copyright notice in the Description page of Project Settings.


#include "TheData.h"

TheData::TheData()
{
	Distance=0;
	Radius=0;
	EmInterference1 = 0;
	EmInterference2=0;
	Angle=0;
	ThreatLevel = 0;
}

TheData::~TheData()
{
}
void TheData::UpdateValue()
{
	ThreatLevel = (800/FMath::Abs(Distance))* 45 +
		FMath::Abs(Angle) / 45 * 8 +
		FMath::Abs(GetTargetPriority(Type)) / 3.0 * 15 +
		FMath::Abs(GetStatusPriority(Status)) / 3.0 * 15 +
		FMath::Abs(EmInterference1) / 400 * 8 +
		FMath::Abs(EmInterference2) / 700 * 8 +
		FMath::Abs(Radius) / 600 * 8;
}