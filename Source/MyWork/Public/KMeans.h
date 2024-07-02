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
    KMeans(int k, int maxIterations, float errorRate) :K(k), MaxIterations(maxIterations),ErrorRate(errorRate) {};
	~KMeans();
public:
    void Run();

private:
    TArray<int> Labels;
    TArray<TheData*> Points;
    TMap<FName,int> Map;
    TheData* OurUnit;
    int K;
    int MaxIterations;
    float ErrorRate;
    //float sigma[9];
public:
    void SetBasicdata(TArray<ABasePawn*> & Aggregate,ABasePawn* &Mine);

    double CalculateDistance(TheData*& FirstPoints, TheData*& SecondPoints);

    void GetAllData(TMap<FName, int>& TheMap) const;
    void KMeansSort(TArray<TheData*>& SuessPoints);
};
