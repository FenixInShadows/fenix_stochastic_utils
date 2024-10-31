// Copyright 2024, Tiannan Chen, All rights reserved.


#include "SelectorUtils.h"
#include "CommonUtils.h"

URandomSelector* USelectorUtils::CreateRandomSelector(const FRandomSelectorConfig& config)
{
	return nullptr;
}

TArray<double> USelectorUtils::GetCumWeights(const TArray<double>& weights)
{
	int32 num = weights.Num();

	double sum_weight = 0.0;
	TArray<double> cum_weights;
	cum_weights.SetNum(num);

	for (int32 i = 0; i < num; i++)
	{
		double temp_weight = FMath::Max(weights[i], 0.0);
		sum_weight += temp_weight;
		cum_weights[i] = sum_weight;
	}

	return cum_weights;
}

int32 USelectorUtils::SelectWithCumWeights(const TArray<double>& cum_weights)
{
	int32 num = cum_weights.Num();
	if (num == 0)
	{
		return -1;
	}
	if (num == 1 && cum_weights[0] > 0.0)
	{
		return 0;
	}

	double sum_weight = cum_weights[num - 1];
	if (sum_weight == 0.0)
	{
		return -1;
	}

	double random_roll = FMath::FRandRange(0.0, sum_weight);
	return UCommonUtils::BinarySearchInSegment(random_roll, cum_weights, 0, num - 1);
}

int32 USelectorUtils::SelectWithWeights(const TArray<double>& weights)
{
	int32 num = weights.Num();
	if (num == 0)
	{
		return -1;
	}
	if (num == 1 && weights[0] > 0.0)
	{
		return 0;
	}

	double sum_weight = 0.0;
	TArray<double> cum_weights;
	cum_weights.SetNum(num);

	for (int32 i = 0; i < num; i++)
	{
		double temp_weight = FMath::Max(weights[i], 0.0);
		sum_weight += temp_weight;
		cum_weights[i] = sum_weight;
	}
	if (sum_weight == 0.0)
	{
		return -1;
	}

	double random_roll = FMath::FRandRange(0.0, sum_weight);
	return UCommonUtils::BinarySearchInSegment(random_roll, cum_weights, 0, num - 1);
}
