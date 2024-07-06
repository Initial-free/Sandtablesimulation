// Fill out your copyright notice in the Description page of Project Settings.


#include "TheData.h"

TheData::TheData()
{
	Distance=0;
	Radius=0;
	EmInterference1 = 0;
	EmInterference2=0;
	Angle=0;
}

TheData::~TheData()
{
}
//������в��
void TheData::UpdateValue()
{
	ThreatLevel = (8000 / (FMath::Abs(Distance) + 1)) * 40 +
		FMath::Abs(Angle) / 45 * 8 +
		FMath::Abs(GetTargetPriority(Type)) / 3.0 * 20 +
		FMath::Abs(GetStatusPriority(Status)) / 3.0 * 20 +
		FMath::Abs(EmInterference1) / 400 * 9 +
		FMath::Abs(EmInterference2) / 700 * 10 +
		FMath::Abs(Radius) / 600 * 14;
}

