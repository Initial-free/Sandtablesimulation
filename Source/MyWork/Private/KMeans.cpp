
#include "KMeans.h"
KMeans::~KMeans()
{
	Points.Empty();
}
PRAGMA_DISABLE_OPTIMIZATION
void KMeans::Run()
{
	TArray<int> Label;
	TArray<TheData*> Centroids;
	bool Sel[101] = {};
	Label.SetNum(Points.Num());
	for (int32 i = 0; i < Points.Num(); i++)
	{
		Label[i] = 0;
	}
	for (int32 i = 1; i <= FMath::Min(Points.Num(), K); i++)
	{
		int k = FMath::Rand() % FMath::Max(Points.Num(), K);
		while (Sel[k] == true)
			k = FMath::Rand() % FMath::Max(Points.Num(), K);
		Centroids.Add(Points[k]);
		Sel[k] = true;

	}
	int aa = 1000;
	while (aa--)
	{

		// 分配每个点到最近的质心
		for (int32 i = 0; i < Points.Num(); i++) {
			//获取最大数
			double MinDistance = std::numeric_limits<double>::max();
			int ClosestCentroid = 0;
			for (int32 j = 0; j < FMath::Min(Points.Num(), K); j++) {
				double Distance = 0;
				Distance += CalculateDistance(Points[i], Centroids[j]);
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
		for (int32 i = 0; i < K; i++)
		{
			TheData* data = new TheData();
			NewCentroids.Add(data);
		}
		TArray<int> NewSize;
		NewSize.SetNum(Points.Num(), 0);
		for (int32 i = 0; i < Points.Num(); ++i) {
			*NewCentroids[Label[i]] += *Points[i];
			NewSize[Label[i]]++;
		}

		for (int i = 0; i < 15; ++i) {
			if (NewSize[i] > 0) {
				*NewCentroids[i] /= NewSize[i];
			}
		}
		Centroids = NewCentroids;
		NewCentroids.Empty();
		NewSize.Empty();
	}
	// 分配每个点到最近的质心
	//while (true)
	//{
	//	MaxIterations--;
	//	if (MaxIterations < 0)
	//		break;
	//	for (int32 i = 0; i < Points.Num(); i++) {
	//		//获取最大数
	//		double MinDistance = std::numeric_limits<double>::max();
	//		int ClosestCentroid = 0;
	//		for (int32 j = 0; j < FMath::Min(Points.Num(), K); j++) {
	//			double Distance = 0;
	//			Distance += CalculateDistance(Points[i], Centroids[j]);
	//			if (Distance < MinDistance) {
	//				MinDistance = Distance;
	//				ClosestCentroid = j;
	//			}
	//		}
	//		if (Label[i] != ClosestCentroid) {
	//			Label[i] = ClosestCentroid;
	//		}
	//	}

	//	// 更新质心
	//	TArray<TheData*> NewCentroids;
	//	for (int32 i = 0; i < K; i++)
	//	{
	//		TheData* data = new TheData();
	//		NewCentroids.Add(data);
	//	}
	//	TArray<int> NewSize;
	//	NewSize.SetNum(Points.Num(), 0);
	//	for (int32 i = 0; i < Points.Num(); ++i) {
	//		*NewCentroids[Label[i]] += *Points[i];
	//		NewSize[Label[i]]++;
	//	}
	//	for (int i = 0; i < 15; ++i) {
	//		if (NewSize[i] > 0) {
	//			*NewCentroids[i] /= NewSize[i];
	//		}
	//	}
	//	for (int i = 0; i < 15; i++) {
	//		if (CalculateDistance(Centroids[i], NewCentroids[i]) < ErrorRate)
	//		{
	//			break;
	//		}
	//	}
	//	Centroids = NewCentroids;
	//	NewCentroids.Empty();
	//	NewSize.Empty();
	//}
	Labels = Label;
	Label.Empty();
	KMeansSort(Labels);
	for (int i = 0; i < K; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("The result %d"),Labels[i]);
	}
}
PRAGMA_ENABLE_OPTIMIZATION
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
			//获取电磁场强度
			data->EmInterference1 = ThePawn->EmInterference1;
			data->EmInterference2 = ThePawn->EmInterference2;
			//获取两点之间的距离
			data->Distance = TempDistance;
			//获取相对偏转角度
			data->Angle = TempAngle;
			//获取方向向量
			data->Direction = TempDirectionToOther;
			//获取类型
			data->Type = ThePawn->Type;
			//获取状态
			data->Status = ThePawn->Status;
			//获取姓名
			data->Name = ThePawn->GetFName();
			this->Points.Add(data);
		}
	}
}
PRAGMA_DISABLE_OPTIMIZATION
double KMeans::CalculateDistance(TheData*& FirstPoints, TheData*& SecondPoints)
{
	double Temp;
	Temp = FMath::Sqrt(
		FMath::Square(FirstPoints->Distance - SecondPoints->Distance) +
		FMath::Square((FirstPoints->Direction - SecondPoints->Direction).Length()) +
		FMath::Square(FirstPoints->Angle - SecondPoints->Angle) +
		FMath::Square(FirstPoints->TypePriority - SecondPoints->TypePriority) +
		FMath::Square(FirstPoints->StatusPriority - SecondPoints->StatusPriority) +
		FMath::Square(FirstPoints->EmInterference1 - SecondPoints->EmInterference1) +
		FMath::Square(FirstPoints->EmInterference2 - SecondPoints->EmInterference2) +
		FMath::Square(FirstPoints->Radius - SecondPoints->Radius)
	);
	return Temp;
}
PRAGMA_ENABLE_OPTIMIZATION
void KMeans::GetAllData(TArray<int> PublicLabels)
{
	PublicLabels = this->Labels;
}
void KMeans::KMeansSort(TArray<int> &SuessPoints)
{
	SuessPoints.HeapSort([](const int&a,const int&b){
		return a < b;
	});
}
