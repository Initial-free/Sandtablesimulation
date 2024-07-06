// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"EnumClass.h"
/**
 * 
 */
class MYWORK_API TheData
{
public:
	TheData();
	~TheData();
public:
	double Distance;
	double Radius;
	double EmInterference1, EmInterference2;
	double Angle;
	TargetType Type;
	float TypePriority = 0;
	CombatStatus Status;
	float StatusPriority = 0;
	FName Name;
	FVector Direction;
	float ThreatLevel;
public:
	void UpdateValue();
	//ÔËËã·ûÖØÔØ
	TheData operator+=(const TheData& a)
	{

		this->Distance += a.Distance;
		this->Radius += a.Radius;
		this->EmInterference1 += a.EmInterference1;
		this->EmInterference2 += a.EmInterference2;
		this->Angle += Angle;
		this->TypePriority += GetTargetPriority(a.Type);
		this->StatusPriority += GetStatusPriority(a.Status);
		this->ThreatLevel += a.ThreatLevel;
		return *this;
	}
	TheData operator/=(int a)
	{
		this->Distance /= a;
		this->Radius /= a;
		this->EmInterference1 /= a;
		this->EmInterference2 /= a;
		this->Angle /= a;
		this->TypePriority /= a;
		this->StatusPriority /= a;
		this->ThreatLevel /= a;
		return *this;
	}
};
