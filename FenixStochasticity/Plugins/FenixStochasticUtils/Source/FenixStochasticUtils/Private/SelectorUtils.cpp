// Copyright 2024, Tiannan Chen, All rights reserved.


#include "SelectorUtils.h"
#include "CommonUtils.h"

//URandomSelector* USelectorUtils::CreateRandomSelector(const FRandomSelectorConfig& Config)
//{
//	return nullptr;
//}

void USelectorUtils::MakeCumulatives(const TArray<double>& Values, TArray<double>& OutCumulatives, double ValueLowerClamp)
{
	const int32 Num = Values.Num();

	OutCumulatives.SetNum(Num);
	double SumValue = 0.0;

	for (int32 Idx = 0; Idx < Num; Idx++)
	{
		const double TempValue = FMath::Max(Values[Idx], ValueLowerClamp);
		SumValue += TempValue;
		OutCumulatives[Idx] = SumValue;
	}
}

void USelectorUtils::MakeCumulativesWithCutoff(const TArray<double>& Values, TArray<double>& OutCumulatives, double ValueLowerClamp, double TotalCutoff)
{
	int32 Num = Values.Num();

	OutCumulatives.SetNum(Num);
	double SumValue = 0.0;

	for (int32 Idx = 0; Idx < Num; Idx++)
	{
		const double TempValue = FMath::Max(Values[Idx], ValueLowerClamp);
		SumValue += TempValue;
		OutCumulatives[Idx] = SumValue;
		if (Idx < Num - 1 && SumValue >= TotalCutoff)  // cutoff
		{
			Num = Idx + 1;
			OutCumulatives.SetNum(Num);
			break;
		}
	}
}

void USelectorUtils::CookWeightOrProbConfigs(const TArray<FWeightOrProbConfig>& RawConfigs, FCookedWeightsOrProbsConfig& OutCookedConfig)
{
	const int32 Num = RawConfigs.Num();

	OutCookedConfig.CumWeightsOrCumProbs.SetNum(Num);
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
		OutCookedConfig.IsProbs = false;
		SumWeight = 0.0;
		for (int32 Idx = 0; Idx < Num; Idx++)
		{
			if (!RawConfigs[Idx].IsProb)
			{
				SumWeight += FMath::Max(RawConfigs[Idx].WeightOrProb, 0.0);
			}
			OutCookedConfig.CumWeightsOrCumProbs[Idx] = SumWeight;
		}
	}
	else if (SumWeight == 0.0 || 1.0 - SumProb < 1e-6)  // pure probabilities
	{
		OutCookedConfig.IsProbs = true;
		SumProb = 0.0;
		for (int32 Idx = 0; Idx < Num; Idx++)
		{
			if (RawConfigs[Idx].IsProb)
			{
				SumProb += FMath::Max(RawConfigs[Idx].WeightOrProb, 0.0);
			}
			// Note: don't do cut off here, as we do not make much assumptions on how the result would be used
			OutCookedConfig.CumWeightsOrCumProbs[Idx] = SumProb;
		}
	}
	else  // mixture: convert to weights
	{
		OutCookedConfig.IsProbs = false;
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
			OutCookedConfig.CumWeightsOrCumProbs[Idx] = SumWeight;
		}
	}
}

int32 USelectorUtils::BPFunc_SelectWithCumWeights(const TArray<double>& CumWeights)
{
	return SelectWithCumWeights(CumWeights);
}

int32 USelectorUtils::BPFunc_SelectWithCumWeightsFromStream(const TArray<double>& CumWeights, FRandomStream& Stream)
{
	return SelectWithCumWeights(CumWeights, &Stream);
}

int32 USelectorUtils::BPFunc_SelectWithWeights(const TArray<double>& Weights)
{
	return SelectWithWeights(Weights);
}

int32 USelectorUtils::BPFunc_SelectWithWeightsFromStream(const TArray<double>& Weights, FRandomStream& Stream)
{
	return SelectWithWeights(Weights, &Stream);
}

int32 USelectorUtils::BPFunc_SelectWithCumProbs(const TArray<double>& CumProbs)
{
	return SelectWithCumProbs(CumProbs);
}

int32 USelectorUtils::BPFunc_SelectWithCumProbsFromStream(const TArray<double>& CumProbs, FRandomStream& Stream)
{
	return SelectWithCumProbs(CumProbs, &Stream);
}

int32 USelectorUtils::BPFunc_SelectWithProbs(const TArray<double>& Probs)
{
	return SelectWithProbs(Probs);
}

int32 USelectorUtils::BPFunc_SelectWithProbsFromStream(const TArray<double>& Probs, FRandomStream& Stream)
{
	return SelectWithProbs(Probs, &Stream);
}

int32 USelectorUtils::BPFunc_SelectWithCookedWeightsOrProbsConfig(const FCookedWeightsOrProbsConfig& CookedConfig)
{
	return SelectWithCookedWeightsOrProbsConfig(CookedConfig);
}

int32 USelectorUtils::BPFunc_SelectWithCookedWeightsOrProbsConfigFromStream(const FCookedWeightsOrProbsConfig& CookedConfig, FRandomStream& Stream)
{
	return SelectWithCookedWeightsOrProbsConfig(CookedConfig, &Stream);
}

int32 USelectorUtils::BPFunc_SelectWithWeightOrProbConfigs(const TArray<FWeightOrProbConfig>& RawConfigs)
{
	return SelectWithWeightOrProbConfigs(RawConfigs);
}

int32 USelectorUtils::BPFunc_SelectWithWeightOrProbConfigsFromStream(const TArray<FWeightOrProbConfig>& RawConfigs, FRandomStream& Stream)
{
	return SelectWithWeightOrProbConfigs(RawConfigs, &Stream);
}

int32 USelectorUtils::SelectWithCumWeights(const TArray<double>& CumWeights, FRandomStream* Stream)
{
	const int32 Num = CumWeights.Num();
	if (Num == 0)
	{
		return -1;
	}
	if (Num == 1)  // for weight (as opposed to probability) we can conveniently make the case for Num == 1 decided quicker
	{
		return CumWeights[0] > 0.0 ? 0 : -1;
	}

	const double SumWeight = CumWeights[Num - 1];
	if (SumWeight == 0.0)
	{
		return -1;
	}

	return SelectWithCumWeightsHelper(CumWeights, Num, SumWeight, Stream);
}

int32 USelectorUtils::SelectWithWeights(const TArray<double>& Weights, FRandomStream* Stream)
{
	const int32 Num = Weights.Num();
	if (Num == 0)
	{
		return -1;
	}
	if (Num == 1)  // for weight (as opposed to probability) we can conveniently make the case for Num == 1 decided quicker
	{
		return Weights[0] > 0.0 ? 0 : -1;
	}

	TArray<double> CumWeights;
	MakeCumulatives(Weights, CumWeights);

	const double SumWeight = CumWeights[Num - 1];
	if (SumWeight == 0.0)
	{
		return -1;
	}

	return SelectWithCumWeightsHelper(CumWeights, Num, SumWeight, Stream);
}

int32 USelectorUtils::SelectWithCumProbs(const TArray<double>& CumProbs, FRandomStream* Stream)
{
	const int32 Num = CumProbs.Num();
	if (Num == 0)
	{
		return -1;
	}

	const double SumProb = CumProbs[Num - 1];
	if (SumProb == 0.0)
	{
		return -1;
	}

	return SelectWithCumProbsHelper(CumProbs, Num, Stream);
}

int32 USelectorUtils::SelectWithProbs(const TArray<double>& Probs, FRandomStream* Stream)
{
	int32 Num = Probs.Num();
	if (Num == 0)
	{
		return -1;
	}

	TArray<double> CumProbs;
	MakeCumulativesWithCutoff(Probs, CumProbs); // can do cutoff here, as these are probabilities, and it's temporary use so there's no risk on changing the array size
	Num = CumProbs.Num(); // recompute as the array size could be cut off above
	
	const double SumProb = CumProbs[Num - 1];
	if (SumProb == 0.0)
	{
		return -1;
	}

	return SelectWithCumProbsHelper(CumProbs, Num, Stream);
}

int32 USelectorUtils::SelectWithCookedWeightsOrProbsConfig(const FCookedWeightsOrProbsConfig& CookedConfig, FRandomStream* Stream)
{
	if (CookedConfig.IsProbs)
	{
		return SelectWithCumProbs(CookedConfig.CumWeightsOrCumProbs, Stream);
	}
	return SelectWithCumWeights(CookedConfig.CumWeightsOrCumProbs, Stream);
}

int32 USelectorUtils::SelectWithWeightOrProbConfigs(const TArray<FWeightOrProbConfig>& RawConfigs, FRandomStream* Stream)
{
	int32 Num = RawConfigs.Num();
	if (Num == 0)
	{
		return -1;
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
		return SelectWithCumWeightsHelper(CumWeights, Num, SumWeight, Stream);
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
		return SelectWithCumProbsHelper(CumProbs, Num, Stream);
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
	return SelectWithCumWeightsHelper(CumWeights, Num, SumWeight, Stream);
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

int32 USelectorUtils::SelectWithCumWeightsHelper(const TArray<double>& CumWeights, const int32 Num, const double SumWeight, FRandomStream* Stream)
{
	const double RandomRoll = UCommonUtils::FRandRangeMaybeWithStream(0.0, SumWeight, Stream);
	int32 SelectedIndex = UCommonUtils::BinarySearchForInsertionInSegment(RandomRoll, CumWeights, 0, Num - 1);

	// guard against rare cases where it rolls exactly sum weight and one or more elements at the end are with zero weights
	if (SelectedIndex == Num - 1)
	{
		while (SelectedIndex > 0 && CumWeights[SelectedIndex] == CumWeights[SelectedIndex - 1])
		{
			SelectedIndex--;
		}
	}

	return SelectedIndex;
}

int32 USelectorUtils::SelectWithCumProbsHelper(const TArray<double>& CumProbs, const int32 Num, FRandomStream* Stream)
{
	const double RandomRoll = UCommonUtils::FRandMaybeWithStream(Stream);
	int32 SelectedIndex = UCommonUtils::BinarySearchForInsertionInSegment(RandomRoll, CumProbs, 0, Num);  // here Num is included to accommodate the case where total prob being not enough

	if (SelectedIndex == Num)
	{
		// guard against cases where the total is approximately 1.0 and it rolls approximately 1.0
		if (1 - CumProbs[Num - 1] < 1e-6)
		{
			SelectedIndex = Num - 1;
			while (SelectedIndex > 0 && CumProbs[SelectedIndex] == CumProbs[SelectedIndex - 1])
			{
				SelectedIndex--;
			}
			return SelectedIndex;
		}

		// it rolls outside the total prob, so return failure
		return -1;
	}

	// guard against rare cases where one or more elements at the end (before cum prob of 1.0) are with zero probs and it rolls 1.0 exactly
	if (SelectedIndex == Num - 1)
	{
		while (SelectedIndex > 0 && CumProbs[SelectedIndex] == CumProbs[SelectedIndex - 1])
		{
			SelectedIndex--;
		}
	}

	return SelectedIndex;
}
