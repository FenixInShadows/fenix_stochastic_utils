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
	int32 selected_index = UCommonUtils::BinarySearchInSegment(random_roll, cum_weights, 0, num - 1);

	// guard against a rare case where it rolls exactly sum_weight and one or more elements at the end are with zero weights
	if (selected_index == num - 1)
	{
		while (selected_index > 0 && cum_weights[selected_index] == cum_weights[selected_index - 1])
		{
			selected_index--;
		}
	}

	return selected_index;
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
	int32 selected_index = UCommonUtils::BinarySearchInSegment(random_roll, cum_weights, 0, num - 1);

	// guard against a rare case where it rolls exactly sum_weight and one or more elements at the end are with zero weights
	if (selected_index == num - 1)
	{
		while (weights[selected_index] == 0.0) // must exist at least one with positive weight
		{
			selected_index--;
		}
	}

	return selected_index;
}
