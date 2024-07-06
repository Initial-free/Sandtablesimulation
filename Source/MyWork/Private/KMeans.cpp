
#include "KMeans.h"
KMeans::~KMeans()
{

}
//Kmeans运行
void KMeans::Run()
{
	TArray<int> Label;
	TArray<TheData*> Centroids;
	bool Sel[101] = {};
	Label.SetNum(Points.Num());
	Map.Empty();
	for (int32 i = 0; i < Points.Num(); i++)
	{
		Label[i] = -1;
	}
	for (int32 i = 1; i <= FMath::Min(Points.Num(), K); i++)
	{
		int k = FMath::Rand() % FMath::Min(Points.Num(), K);
		while (Sel[k] == true)
			k = FMath::Rand() % FMath::Min(Points.Num(), K);
		Centroids.Add(Points[k]);
		Sel[k] = true;

	}

	KMeansSort(Centroids);
	while (true)
	{
		MaxIterations--;
		if (MaxIterations < 0)
			break;
		//计算距离质心
		for (int32 i = 0; i < Points.Num(); i++) {
			//获取最大数
			double MinDistance = std::numeric_limits<double>::max();
			int ClosestCentroid = -1;
			for (int32 j = 0; j < Centroids.Num(); j++) {
				double Distance = 0;
				Distance += CalculateDistance(Points[i],Centroids[j]);
				if (Distance < MinDistance) {
					MinDistance = Distance;
					ClosestCentroid = j;
				}
			}
			if (Label[i] != ClosestCentroid) {
				Label[i] = ClosestCentroid;
			}
		}

		// 更新质心
		TArray<TheData*> NewCentroids;
		for (int32 i = 0; i < Centroids.Num(); i++)
		{
			TheData* data = new TheData();
			data->Type = Centroids[i]->Type;
			data->Status = Centroids[i]->Status;
			NewCentroids.Add(data);
		}
		TArray<int> NewSize;
		NewSize.SetNum(Points.Num(), 0);
		for (int32 i = 0; i < Points.Num(); ++i) {
			*NewCentroids[Label[i]] += *Points[i];
			NewSize[Label[i]]++;
		}
		for (int i = 0; i < Centroids.Num(); ++i) {
			if (NewSize[i] > 0) {
				*NewCentroids[i] /= NewSize[i];
			}
		}
		bool IsBreak = true; // 默认设置为True，如果所有质心都未收敛，则继续迭代
		for (int i = 0; i < Centroids.Num(); i++) {
			if (CalculateDistance(Centroids[i], NewCentroids[i]) >= ErrorRate) {
				IsBreak = false; // 如果有任何一个质心未收敛，则设置为False
				break;
			}
		}
		if (IsBreak) {
			break; // 如果所有质心都收敛，则退出循环
		}
		Centroids = NewCentroids;
		KMeansSort(Centroids);
		NewCentroids.Empty();
		NewSize.Empty();
	}
	Labels = Label;
	Label.Empty();
	int j = 0;
	for (TheData* Enemy : Points)
	{
		if (Enemy != nullptr)
		{
			Map.Add(Enemy->Name, Labels[j++]);
		}
	}
	Points.Empty();
	Centroids.Empty();
}
void KMeans::SetBasicdata(TArray<ABasePawn*>& Aggregate, ABasePawn*& Mine)
{
	for (ABasePawn* ThePawn : Aggregate)
	{
		if (ThePawn)
		{
			TheData* data = new TheData;
			FVector TempTargetDistance = ThePawn->GetTargetLocation() - Mine->GetTargetLocation();
			double TempDistance = TempTargetDistance.Length();
			//计算两个之间的方向向量
			FVector TempDirectionToOther = (ThePawn->GetActorLocation() - Mine->GetActorLocation()).GetSafeNormal();
			// 获取自身的朝向向量
			FVector TempForwardVector = Mine->GetActorForwardVector();

			// 计算两个向量之间的夹角
			float TempAngle = FMath::Acos(FVector::DotProduct(TempForwardVector, TempDirectionToOther)) * (180.0f / PI);
			if (TempAngle == FMath::Clamp(TempAngle, PI / 4, PI *3 / 4))
				TempAngle -= PI / 4;
			else if(TempAngle == FMath::Clamp(TempAngle,0, PI / 4))
				TempAngle += PI / 4;
			//获取电磁场强度
			data->EmInterference1 = ThePawn->EmInterference1;
			data->EmInterference2 = ThePawn->EmInterference2;
			//获取两点之间的距离
			data->Distance = TempDistance;
			//获取相对偏转角度
			data->Angle = TempAngle;
			//获取半径
			data->Radius = ThePawn->Radius;
			//获取方向向量
			data->Direction = TempDirectionToOther;
			//获取类型
			data->Type = ThePawn->Type;
			//获取状态
			data->Status = ThePawn->Status;
			//获取姓名
			data->Name = ThePawn->GetFName();
			data->UpdateValue();
			ThePawn->Threatlevel = data->ThreatLevel;
			this->Points.Add(data);
		}
	}
	OurUnit = new TheData();
	OurUnit->Name = Mine->GetFName();
}
PRAGMA_DISABLE_OPTIMIZATION
double KMeans::CalculateDistance(TheData*& FirstPoints, TheData*& SecondPoints)
{
	//计算距离
	double Temp;
	Temp = (8000 / (FMath::Abs(FirstPoints->Distance-SecondPoints->Distance)+1)) * 40 +
		FMath::Abs(FirstPoints->Angle - SecondPoints->Angle) / 45 * 8 +
		FMath::Abs(FirstPoints->TypePriority - SecondPoints->TypePriority) / 3.0 * 20 +
		FMath::Abs(FirstPoints->StatusPriority - SecondPoints->StatusPriority) / 3.0 * 20 +
		FMath::Abs(FirstPoints->EmInterference1 - SecondPoints->EmInterference1) / 400 * 9 +
		FMath::Abs(FirstPoints->EmInterference2 - SecondPoints->EmInterference2) / 700 * 10 +
		FMath::Abs(FirstPoints->Radius - SecondPoints->Radius) / 600 * 14;
	return Temp;
}
PRAGMA_ENABLE_OPTIMIZATION
void KMeans::GetAllData(TMap<FName, int>& TheMap) const
{
	TheMap = Map;
}
void KMeans::KMeansSort(TArray<TheData*> &SuessPoints)
{
	SuessPoints.HeapSort([](const TheData&a,const TheData&b){
		return a.ThreatLevel > b.ThreatLevel;
	});
}
