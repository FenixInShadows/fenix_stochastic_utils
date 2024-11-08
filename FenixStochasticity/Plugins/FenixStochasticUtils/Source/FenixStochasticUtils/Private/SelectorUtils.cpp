// Copyright 2024, Tiannan Chen, All rights reserved.


#include "SelectorUtils.h"
#include "CommonUtils.h"

URandomSelector* USelectorUtils::CreateRandomSelector(const FRandomSelectorConfig& config)
{
	return nullptr;
}

TArray<double> USelectorUtils::MakeCumWeights(const TArray<double>& weights)
{
	const int32 num = weights.Num();

	double sum_weight = 0.0;
	TArray<double> cum_weights;
	cum_weights.SetNum(num);

	for (int32 i = 0; i < num; i++)
	{
		const double temp_weight = FMath::Max(weights[i], 0.0);
		sum_weight += temp_weight;
		cum_weights[i] = sum_weight;
	}

	return cum_weights;
}

int32 USelectorUtils::SelectWithCumWeights(const TArray<double>& cum_weights)
{
	const int32 num = cum_weights.Num();
	if (num == 0)
	{
		return -1;
	}
	if (num == 1)
	{
		return cum_weights[0] > 0.0 ? 0 : -1;
	}

	const double sum_weight = cum_weights[num - 1];
	if (sum_weight == 0.0)
	{
		return -1;
	}

	return SelectWithCumWeightsHelper(cum_weights, num, sum_weight);
}

int32 USelectorUtils::SelectWithWeights(const TArray<double>& weights)
{
	const int32 num = weights.Num();
	if (num == 0)
	{
		return -1;
	}
	if (num == 1)
	{
		return weights[0] > 0.0 ? 0 : -1;
	}

	double sum_weight = 0.0;
	TArray<double> cum_weights;
	cum_weights.SetNum(num);

	for (int32 i = 0; i < num; i++)
	{
		const double temp_weight = FMath::Max(weights[i], 0.0);
		sum_weight += temp_weight;
		cum_weights[i] = sum_weight;
	}
	if (sum_weight == 0.0)
	{
		return -1;
	}

	return SelectWithCumWeightsHelper(cum_weights, num, sum_weight);
}

TArray<double> USelectorUtils::MakeCumProbs(const TArray<double>& probs)
{
	int32 num = probs.Num();

	double sum_prob = 0.0;
	TArray<double> cum_probs;
	cum_probs.SetNum(num);

	for (int32 i = 0; i < num; i++)
	{
		const double temp_prob = FMath::Max(probs[i], 0.0);
		sum_prob += temp_prob;
		cum_probs[i] = sum_prob;
		if (i < num - 1 && sum_prob >= 1.0)  // cut off at cum prob of 1.0
		{
			num = i + 1;
			cum_probs.SetNum(num);
			break;
		}
	}

	return cum_probs;
}

int32 USelectorUtils::SelectWithCumProbs(const TArray<double>& cum_probs)
{
	const int32 num = cum_probs.Num();
	if (num == 0)
	{
		return -1;
	}
	if (num == 1)
	{
		return cum_probs[0] > 0.0 ? 0 : -1;
	}

	const double sum_probs = cum_probs[num - 1];
	if (sum_probs == 0.0)
	{
		return -1;
	}

	return SelectWithCumProbsHelper(cum_probs, num);
}

int32 USelectorUtils::SelectWithProbs(const TArray<double>& probs)
{
	int32 num = probs.Num();
	if (num == 0)
	{
		return -1;
	}
	if (num == 1)
	{
		return probs[0] > 0.0 ? 0 : -1;
	}

	double sum_prob = 0.0;
	TArray<double> cum_probs;
	cum_probs.SetNum(num);

	for (int32 i = 0; i < num; i++)
	{
		const double temp_prob = FMath::Max(probs[i], 0.0);
		sum_prob += temp_prob;
		cum_probs[i] = sum_prob;
		if (i < num - 1 && sum_prob >= 1.0)  // cut off at cum prob of 1.0
		{
			num = i + 1;
			cum_probs.SetNum(num);
			break;
		}
	}
	if (sum_prob == 0.0)
	{
		return -1;
	}

	return SelectWithCumProbsHelper(cum_probs, num);
}

FCookedWeightsOrProbsConfig USelectorUtils::CookWeightOrProbConfigs(const TArray<FWeightOrProbConfig>& raw_configs)
{
	int32 num = raw_configs.Num();
	FCookedWeightsOrProbsConfig cooked_config;
	cooked_config.CumWeightsOrCumProbs.SetNum(num);

	double sum_weight = 0.0;
	double sum_prob = 0.0;
	for (int32 i = 0; i < num; i++)
	{
		if (raw_configs[i].IsProb)
		{
			sum_prob += FMath::Max(raw_configs[i].WeightOrProb, 0.0);
		}
		else
		{
			sum_weight += FMath::Max(raw_configs[i].WeightOrProb, 0.0);
		}
	}

	if (sum_prob == 0.0)  // pure weights
	{
		cooked_config.IsProbs = false;
		sum_weight = 0.0;
		for (int32 i = 0; i < num; i++)
		{
			if (!raw_configs[i].IsProb)
			{
				sum_weight += FMath::Max(raw_configs[i].WeightOrProb, 0.0);
			}
			cooked_config.CumWeightsOrCumProbs[i] = sum_weight;
		}
	}
	else if (sum_weight == 0.0 || 1.0 - sum_prob < 1e-6)  // pure probabilities
	{
		cooked_config.IsProbs = true;
		sum_prob = 0.0;
		for (int32 i = 0; i < num; i++)
		{
			if (raw_configs[i].IsProb)
			{
				sum_prob += FMath::Max(raw_configs[i].WeightOrProb, 0.0);
			}
			cooked_config.CumWeightsOrCumProbs[i] = sum_prob;
			if (i < num - 1 && sum_prob >= 1.0)  // cut off at cum prob of 1.0
			{
				num = i + 1;
				cooked_config.CumWeightsOrCumProbs.SetNum(num);
				break;
			}
		}
	}
	else  // mixture: convert to weights
	{
		cooked_config.IsProbs = false;
		const double weight_factor = sum_weight / (1.0 - sum_prob);
		sum_weight = 0.0;
		for (int32 i = 0; i < num; i++)
		{
			if (raw_configs[i].IsProb)
			{
				sum_weight += FMath::Max(raw_configs[i].WeightOrProb, 0.0) * weight_factor;
			}
			else
			{
				sum_weight += FMath::Max(raw_configs[i].WeightOrProb, 0.0);
			}
			cooked_config.CumWeightsOrCumProbs[i] = sum_weight;
		}
	}

	return cooked_config;
}

int32 USelectorUtils::SelectWithCookedWeightsOrProbsConfig(const FCookedWeightsOrProbsConfig& cooked_config)
{
	if (cooked_config.IsProbs)
	{
		return SelectWithCumProbs(cooked_config.CumWeightsOrCumProbs);
	}
	return SelectWithCumWeights(cooked_config.CumWeightsOrCumProbs);
}

int32 USelectorUtils::SelectWithWeightOrProbConfigs(const TArray<FWeightOrProbConfig>& raw_configs)
{
	int32 num = raw_configs.Num();
	if (num == 0)
	{
		return -1;
	}
	if (num == 1)
	{
		return raw_configs[0].WeightOrProb > 0.0 ? 0 : -1;
	}

	double sum_weight = 0.0;
	double sum_prob = 0.0;
	for (int32 i = 0; i < num; i++)
	{
		if (raw_configs[i].IsProb)
		{
			sum_prob += FMath::Max(raw_configs[i].WeightOrProb, 0.0);
		}
		else
		{
			sum_weight += FMath::Max(raw_configs[i].WeightOrProb, 0.0);
		}
	}

	// pure weights
	if (sum_prob == 0.0) 
	{
		if (sum_weight == 0.0)
		{
			return -1;
		}

		sum_weight = 0.0;
		TArray<double> cum_weights;
		cum_weights.SetNum(num);
		for (int32 i = 0; i < num; i++)
		{
			if (!raw_configs[i].IsProb)
			{
				sum_weight += FMath::Max(raw_configs[i].WeightOrProb, 0.0);
			}
			cum_weights[i] = sum_weight;
		}
		return SelectWithCumWeightsHelper(cum_weights, num, sum_weight);
	}

	// pure probabilities
	if (sum_weight == 0.0 || 1.0 - sum_prob < 1e-6)
	{
		sum_prob = 0.0;
		TArray<double> cum_probs;
		cum_probs.SetNum(num);
		for (int32 i = 0; i < num; i++)
		{
			if (raw_configs[i].IsProb)
			{
				sum_prob += FMath::Max(raw_configs[i].WeightOrProb, 0.0);
			}
			cum_probs[i] = sum_prob;
			if (i < num - 1 && sum_prob >= 1.0)  // cut off at cum prob of 1.0
			{
				num = i + 1;
				cum_probs.SetNum(num);
				break;
			}
		}
		return SelectWithCumProbsHelper(cum_probs, num);
	}

	// mixture: convert to weights
	const double weight_factor = sum_weight / (1.0 - sum_prob);
	sum_weight = 0.0;
	TArray<double> cum_weights;
	cum_weights.SetNum(num);
	for (int32 i = 0; i < num; i++)
	{
		if (raw_configs[i].IsProb)
		{
			sum_weight += FMath::Max(raw_configs[i].WeightOrProb, 0.0) * weight_factor;
		}
		else
		{
			sum_weight += FMath::Max(raw_configs[i].WeightOrProb, 0.0);
		}
		cum_weights[i] = sum_weight;
	}
	return SelectWithCumWeightsHelper(cum_weights, num, sum_weight);
}

int32 USelectorUtils::SelectWithCumWeightsHelper(const TArray<double>& cum_weights, const int32 num, const double sum_weight)
{
	const double random_roll = FMath::FRandRange(0.0, sum_weight);
	int32 selected_index = UCommonUtils::BinarySearchForInsertionInSegment(random_roll, cum_weights, 0, num - 1);

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

int32 USelectorUtils::SelectWithCumProbsHelper(const TArray<double>& cum_probs, const int32 num)
{
	const double random_roll = FMath::FRand();
	int32 selected_index = UCommonUtils::BinarySearchForInsertionInSegment(random_roll, cum_probs, 0, num - 1);

	// guard against cases where one or more elements at the end (before cum prob of 1.0) are with zero probs but is selected due to padding effect etc.
	if (selected_index == num - 1)
	{
		while (selected_index > 0 && cum_probs[selected_index] == cum_probs[selected_index - 1])
		{
			selected_index--;
		}
	}

	return selected_index;
}
