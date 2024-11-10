// Copyright 2024, Tiannan Chen, All rights reserved.


#include "SelectorUtils.h"
#include "CommonUtils.h"

URandomSelector* USelectorUtils::CreateRandomSelector(const FRandomSelectorConfig& Config)
{
	return nullptr;
}

TArray<double> USelectorUtils::MakeCumWeights(const TArray<double>& Weights)
{
	const int32 Num = Weights.Num();

	double SumWeight = 0.0;
	TArray<double> CumWeights;
	CumWeights.SetNum(Num);

	for (int32 Idx = 0; Idx < Num; Idx++)
	{
		const double TempWeight = FMath::Max(Weights[Idx], 0.0);
		SumWeight += TempWeight;
		CumWeights[Idx] = SumWeight;
	}

	return CumWeights;
}

int32 USelectorUtils::SelectWithCumWeights(const TArray<double>& CumWeights)
{
	const int32 Num = CumWeights.Num();
	if (Num == 0)
	{
		return -1;
	}
	if (Num == 1)
	{
		return CumWeights[0] > 0.0 ? 0 : -1;
	}

	const double SumWeight = CumWeights[Num - 1];
	if (SumWeight == 0.0)
	{
		return -1;
	}

	return SelectWithCumWeightsHelper(CumWeights, Num, SumWeight);
}

int32 USelectorUtils::SelectWithWeights(const TArray<double>& Weights)
{
	const int32 Num = Weights.Num();
	if (Num == 0)
	{
		return -1;
	}
	if (Num == 1)
	{
		return Weights[0] > 0.0 ? 0 : -1;
	}

	double SumWeight = 0.0;
	TArray<double> CumWeights;
	CumWeights.SetNum(Num);

	for (int32 Idx = 0; Idx < Num; Idx++)
	{
		const double TempWeight = FMath::Max(Weights[Idx], 0.0);
		SumWeight += TempWeight;
		CumWeights[Idx] = SumWeight;
	}
	if (SumWeight == 0.0)
	{
		return -1;
	}

	return SelectWithCumWeightsHelper(CumWeights, Num, SumWeight);
}

TArray<double> USelectorUtils::MakeCumProbs(const TArray<double>& Probs)
{
	int32 Num = Probs.Num();

	double SumProb = 0.0;
	TArray<double> CumProbs;
	CumProbs.SetNum(Num);

	for (int32 Idx = 0; Idx < Num; Idx++)
	{
		const double TempProb = FMath::Max(Probs[Idx], 0.0);
		SumProb += TempProb;
		CumProbs[Idx] = SumProb;
		if (Idx < Num - 1 && SumProb >= 1.0)  // cut off at cum prob of 1.0
		{
			Num = Idx + 1;
			CumProbs.SetNum(Num);
			break;
		}
	}

	return CumProbs;
}

int32 USelectorUtils::SelectWithCumProbs(const TArray<double>& CumProbs)
{
	const int32 Num = CumProbs.Num();
	if (Num == 0)
	{
		return -1;
	}
	if (Num == 1)
	{
		return CumProbs[0] > 0.0 ? 0 : -1;
	}

	const double SumProbs = CumProbs[Num - 1];
	if (SumProbs == 0.0)
	{
		return -1;
	}

	return SelectWithCumProbsHelper(CumProbs, Num);
}

int32 USelectorUtils::SelectWithProbs(const TArray<double>& Probs)
{
	int32 Num = Probs.Num();
	if (Num == 0)
	{
		return -1;
	}
	if (Num == 1)
	{
		return Probs[0] > 0.0 ? 0 : -1;
	}

	double SumProb = 0.0;
	TArray<double> CumProbs;
	CumProbs.SetNum(Num);

	for (int32 Idx = 0; Idx < Num; Idx++)
	{
		const double TempProb = FMath::Max(Probs[Idx], 0.0);
		SumProb += TempProb;
		CumProbs[Idx] = SumProb;
		if (Idx < Num - 1 && SumProb >= 1.0)  // cut off at cum prob of 1.0
		{
			Num = Idx + 1;
			CumProbs.SetNum(Num);
			break;
		}
	}
	if (SumProb == 0.0)
	{
		return -1;
	}

	return SelectWithCumProbsHelper(CumProbs, Num);
}

FCookedWeightsOrProbsConfig USelectorUtils::CookWeightOrProbConfigs(const TArray<FWeightOrProbConfig>& RawConfigs)
{
	int32 Num = RawConfigs.Num();
	FCookedWeightsOrProbsConfig CookedConfig;
	CookedConfig.CumWeightsOrCumProbs.SetNum(Num);

	double SumWeight = 0.0;
	double SumProb = 0.0;
	for (int32 Idx = 0; Idx < Num; Idx++)
	{
		if (RawConfigs[Idx].IsProb)
		{
			SumProb += FMath::Max(RawConfigs[Idx].WeightOrProb, 0.0);
		}
		else
		{
			SumWeight += FMath::Max(RawConfigs[Idx].WeightOrProb, 0.0);
		}
	}

	if (SumProb == 0.0)  // pure weights
	{
		CookedConfig.IsProbs = false;
		SumWeight = 0.0;
		for (int32 Idx = 0; Idx < Num; Idx++)
		{
			if (!RawConfigs[Idx].IsProb)
			{
				SumWeight += FMath::Max(RawConfigs[Idx].WeightOrProb, 0.0);
			}
			CookedConfig.CumWeightsOrCumProbs[Idx] = SumWeight;
		}
	}
	else if (SumWeight == 0.0 || 1.0 - SumProb < 1e-6)  // pure probabilities
	{
		CookedConfig.IsProbs = true;
		SumProb = 0.0;
		for (int32 Idx = 0; Idx < Num; Idx++)
		{
			if (RawConfigs[Idx].IsProb)
			{
				SumProb += FMath::Max(RawConfigs[Idx].WeightOrProb, 0.0);
			}
			CookedConfig.CumWeightsOrCumProbs[Idx] = SumProb;
			if (Idx < Num - 1 && SumProb >= 1.0)  // cut off at cum prob of 1.0
			{
				Num = Idx + 1;
				CookedConfig.CumWeightsOrCumProbs.SetNum(Num);
				break;
			}
		}
	}
	else  // mixture: convert to weights
	{
		CookedConfig.IsProbs = false;
		const double WeightFactor = SumWeight / (1.0 - SumProb);
		SumWeight = 0.0;
		for (int32 Idx = 0; Idx < Num; Idx++)
		{
			if (RawConfigs[Idx].IsProb)
			{
				SumWeight += FMath::Max(RawConfigs[Idx].WeightOrProb, 0.0) * WeightFactor;
			}
			else
			{
				SumWeight += FMath::Max(RawConfigs[Idx].WeightOrProb, 0.0);
			}
			CookedConfig.CumWeightsOrCumProbs[Idx] = SumWeight;
		}
	}

	return CookedConfig;
}

int32 USelectorUtils::SelectWithCookedWeightsOrProbsConfig(const FCookedWeightsOrProbsConfig& CookedConfig)
{
	if (CookedConfig.IsProbs)
	{
		return SelectWithCumProbs(CookedConfig.CumWeightsOrCumProbs);
	}
	return SelectWithCumWeights(CookedConfig.CumWeightsOrCumProbs);
}

int32 USelectorUtils::SelectWithWeightOrProbConfigs(const TArray<FWeightOrProbConfig>& RawConfigs)
{
	int32 Num = RawConfigs.Num();
	if (Num == 0)
	{
		return -1;
	}
	if (Num == 1)
	{
		return RawConfigs[0].WeightOrProb > 0.0 ? 0 : -1;
	}

	double SumWeight = 0.0;
	double SumProb = 0.0;
	for (int32 Idx = 0; Idx < Num; Idx++)
	{
		if (RawConfigs[Idx].IsProb)
		{
			SumProb += FMath::Max(RawConfigs[Idx].WeightOrProb, 0.0);
		}
		else
		{
			SumWeight += FMath::Max(RawConfigs[Idx].WeightOrProb, 0.0);
		}
	}

	// pure weights
	if (SumProb == 0.0) 
	{
		if (SumWeight == 0.0)
		{
			return -1;
		}

		SumWeight = 0.0;
		TArray<double> CumWeights;
		CumWeights.SetNum(Num);
		for (int32 Idx = 0; Idx < Num; Idx++)
		{
			if (!RawConfigs[Idx].IsProb)
			{
				SumWeight += FMath::Max(RawConfigs[Idx].WeightOrProb, 0.0);
			}
			CumWeights[Idx] = SumWeight;
		}
		return SelectWithCumWeightsHelper(CumWeights, Num, SumWeight);
	}

	// pure probabilities
	if (SumWeight == 0.0 || 1.0 - SumProb < 1e-6)
	{
		SumProb = 0.0;
		TArray<double> CumProbs;
		CumProbs.SetNum(Num);
		for (int32 Idx = 0; Idx < Num; Idx++)
		{
			if (RawConfigs[Idx].IsProb)
			{
				SumProb += FMath::Max(RawConfigs[Idx].WeightOrProb, 0.0);
			}
			CumProbs[Idx] = SumProb;
			if (Idx < Num - 1 && SumProb >= 1.0)  // cut off at cum prob of 1.0
			{
				Num = Idx + 1;
				CumProbs.SetNum(Num);
				break;
			}
		}
		return SelectWithCumProbsHelper(CumProbs, Num);
	}

	// mixture: convert to weights
	const double WeightFactor = SumWeight / (1.0 - SumProb);
	SumWeight = 0.0;
	TArray<double> CumWeights;
	CumWeights.SetNum(Num);
	for (int32 Idx = 0; Idx < Num; Idx++)
	{
		if (RawConfigs[Idx].IsProb)
		{
			SumWeight += FMath::Max(RawConfigs[Idx].WeightOrProb, 0.0) * WeightFactor;
		}
		else
		{
			SumWeight += FMath::Max(RawConfigs[Idx].WeightOrProb, 0.0);
		}
		CumWeights[Idx] = SumWeight;
	}
	return SelectWithCumWeightsHelper(CumWeights, Num, SumWeight);
}

void USelectorUtils::GetMiddleItem(const TArray<int32>& TargetArray, int32& Item)
{
	// We should never hit these! They're stubs to avoid NoExport on the class. Call the Generic* equivalent instead.
	check(0);
}

void USelectorUtils::Generic_GetMiddleItem(void* TargetArray, const FArrayProperty* ArrayProp, void* OutItem)
{
	if (TargetArray)
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
		FProperty* InnerProp = ArrayProp->Inner;

		const int32 Num = ArrayHelper.Num();
		if (ArrayHelper.Num() > 0)
		{
			InnerProp->CopySingleValueToScriptVM(OutItem, ArrayHelper.GetRawPtr(Num / 2));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Red, "Warning: empty array does not have middle element");
			InnerProp->InitializeValue(OutItem);
		}
	}
}

int32 USelectorUtils::SelectWithCumWeightsHelper(const TArray<double>& CumWeights, const int32 Num, const double SumWeight)
{
	const double RandomRoll = FMath::FRandRange(0.0, SumWeight);
	int32 SelectedIndex = UCommonUtils::BinarySearchForInsertionInSegment(RandomRoll, CumWeights, 0, Num - 1);

	// guard against a rare case where it rolls exactly sum weight and one or more elements at the end are with zero weights
	if (SelectedIndex == Num - 1)
	{
		while (SelectedIndex > 0 && CumWeights[SelectedIndex] == CumWeights[SelectedIndex - 1])
		{
			SelectedIndex--;
		}
	}

	return SelectedIndex;
}

int32 USelectorUtils::SelectWithCumProbsHelper(const TArray<double>& CumProbs, const int32 Num)
{
	const double RandomRoll = FMath::FRand();
	int32 SelectedIndex = UCommonUtils::BinarySearchForInsertionInSegment(RandomRoll, CumProbs, 0, Num - 1);

	// guard against cases where one or more elements at the end (before cum prob of 1.0) are with zero probs but is selected due to padding effect etc.
	if (SelectedIndex == Num - 1)
	{
		while (SelectedIndex > 0 && CumProbs[SelectedIndex] == CumProbs[SelectedIndex - 1])
		{
			SelectedIndex--;
		}
	}

	return SelectedIndex;
}
