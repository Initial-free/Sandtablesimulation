// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "VehiclePawn.h"
#include "FighterPawn.h"
#include "BasePawn.h"
#include "TheData.h"
#include "CoreMinimal.h"
/**
 * 
 */
class MYWORK_API KMeans
{
public:
    KMeans(int k, int maxIterations, double errorRate) :K(k), MaxIterations(maxIterations),ErrorRate(errorRate) {};
	~KMeans();
public:
    void Run();

private:
    TArray<int> Labels;
    TArray<TheData*> Points;
    int K;
    int MaxIterations;
    double ErrorRate;
public:
    void SetBasicdata(TArray<ABasePawn*> & Aggregate,ABasePawn* &Mine);

    double CalculateDistance(TheData*& FirstPoints, TheData*& SecondPoints);

    void GetAllData(TArray<int> PublicLabels);
    void KMeansSort(TArray<int> &SuessPoints);
};
