// Copyright 2025, Tiannan Chen, All rights reserved.


#include "SelectorUtils.h"
#include "CommonUtils.h"
#include "Engine/DataTable.h"

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
		SumValue += FMath::Max(Values[Idx], ValueLowerClamp);
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
		SumValue += FMath::Max(Values[Idx], ValueLowerClamp);
		OutCumulatives[Idx] = SumValue;
		if (Idx < Num - 1 && SumValue >= TotalCutoff)  // cutoff
		{
			Num = Idx + 1;
			OutCumulatives.SetNum(Num);
			break;
		}
	}
}

void USelectorUtils::CookSelectorDistribution(const TArray<FWeightOrProbEntry>& Entries, FCookedSelectorDistribution& OutDistribution)
{
	const int32 Num = Entries.Num();

	OutDistribution.CumWeightsOrCumProbs.SetNum(Num);
	double SumWeight = 0.0;
	double SumProb = 0.0;

	for (int32 Idx = 0; Idx < Num; Idx++)
	{
		if (Entries[Idx].bIsProb)
		{
			SumProb += FMath::Max(Entries[Idx].WeightOrProb, 0.0);
		}
		else
		{
			SumWeight += FMath::Max(Entries[Idx].WeightOrProb, 0.0);
		}
	}

	if (SumProb == 0.0)  // pure weights
	{
		OutDistribution.bIsProbs = false;
		SumWeight = 0.0;
		for (int32 Idx = 0; Idx < Num; Idx++)
		{
			if (!Entries[Idx].bIsProb)
			{
				SumWeight += FMath::Max(Entries[Idx].WeightOrProb, 0.0);
			}
			OutDistribution.CumWeightsOrCumProbs[Idx] = SumWeight;
		}
	}
	else if (SumWeight == 0.0 || 1.0 - SumProb < 1e-6)  // pure probabilities
	{
		OutDistribution.bIsProbs = true;
		SumProb = 0.0;
		for (int32 Idx = 0; Idx < Num; Idx++)
		{
			if (Entries[Idx].bIsProb)
			{
				SumProb += FMath::Max(Entries[Idx].WeightOrProb, 0.0);
			}
			// Note: don't do cut off here, as we do not make much assumptions on how the result would be used
			OutDistribution.CumWeightsOrCumProbs[Idx] = SumProb;
		}
	}
	else  // mixture: convert to weights
	{
		OutDistribution.bIsProbs = false;
		const double WeightFactor = SumWeight / (1.0 - SumProb);
		SumWeight = 0.0;
		for (int32 Idx = 0; Idx < Num; Idx++)
		{
			if (Entries[Idx].bIsProb)
			{
				SumWeight += FMath::Max(Entries[Idx].WeightOrProb, 0.0) * WeightFactor;
			}
			else
			{
				SumWeight += FMath::Max(Entries[Idx].WeightOrProb, 0.0);
			}
			OutDistribution.CumWeightsOrCumProbs[Idx] = SumWeight;
		}
	}
}

void USelectorUtils::GetWeightOrProbEntriesFromDataTable(const UDataTable* DataTable, TArray<FWeightOrProbEntry>& OutEntries, const FName WeightOrProbPropertyName, const FName IsProbPropertyName)
{
	OutEntries.Empty();
	if (DataTable && WeightOrProbPropertyName != NAME_None && IsProbPropertyName != NAME_None)
	{
		const FProperty* WeightOrProbColumnProperty = DataTable->FindTableProperty(WeightOrProbPropertyName);
		const FProperty* IsProbColumnProperty = DataTable->FindTableProperty(IsProbPropertyName);
		if (WeightOrProbColumnProperty && IsProbColumnProperty)
		{
			const FNumericProperty* WeightOrProbNumericProperty = CastField<FNumericProperty>(WeightOrProbColumnProperty);
			const FBoolProperty* IsProbBoolProperty = CastField<FBoolProperty>(IsProbColumnProperty);
			if (WeightOrProbNumericProperty && IsProbBoolProperty)
			{
				for (auto RowIt = DataTable->GetRowMap().CreateConstIterator(); RowIt; ++RowIt)
				{
					uint8* RowData = RowIt.Value();
					FWeightOrProbEntry Entry =
					{
						UCommonUtils::GetFloatingPointPropertyValue_InContainer(WeightOrProbNumericProperty, RowData),
						IsProbBoolProperty->GetPropertyValue_InContainer(RowData)
					};
					OutEntries.Add(Entry);
				}
			}
		}
	}
}

int32 USelectorUtils::BPFunc_SelectWithCumWeights(const TArray<double>& CumWeights)
{
	return SelectWithCumWeights(CumWeights);
}

int32 USelectorUtils::BPFunc_SelectWithCumWeightsFromStream(const TArray<double>& CumWeights, const FRandomStream& RandomStream)
{
	return SelectWithCumWeights(CumWeights, &RandomStream);
}

int32 USelectorUtils::BPFunc_SelectWithWeights(const TArray<double>& Weights)
{
	return SelectWithWeights(Weights);
}

int32 USelectorUtils::BPFunc_SelectWithWeightsFromStream(const TArray<double>& Weights, const FRandomStream& RandomStream)
{
	return SelectWithWeights(Weights, &RandomStream);
}

int32 USelectorUtils::BPFunc_SelectWithCumProbs(const TArray<double>& CumProbs)
{
	return SelectWithCumProbs(CumProbs);
}

int32 USelectorUtils::BPFunc_SelectWithCumProbsFromStream(const TArray<double>& CumProbs, const FRandomStream& RandomStream)
{
	return SelectWithCumProbs(CumProbs, &RandomStream);
}

int32 USelectorUtils::BPFunc_SelectWithProbs(const TArray<double>& Probs)
{
	return SelectWithProbs(Probs);
}

int32 USelectorUtils::BPFunc_SelectWithProbsFromStream(const TArray<double>& Probs, const FRandomStream& RandomStream)
{
	return SelectWithProbs(Probs, &RandomStream);
}

int32 USelectorUtils::BPFunc_SelectWithCookedDistribution(const FCookedSelectorDistribution& Distribution)
{
	return SelectWithCookedDistribution(Distribution);
}

int32 USelectorUtils::BPFunc_SelectWithCookedDistributionFromStream(const FCookedSelectorDistribution& Distribution, const FRandomStream& RandomStream)
{
	return SelectWithCookedDistribution(Distribution, &RandomStream);
}

int32 USelectorUtils::BPFunc_SelectWithWeightOrProbEntries(const TArray<FWeightOrProbEntry>& Entries)
{
	return SelectWithWeightOrProbEntries(Entries);
}

int32 USelectorUtils::BPFunc_SelectWithWeightOrProbEntriesFromStream(const TArray<FWeightOrProbEntry>& Entries, const FRandomStream& RandomStream)
{
	return SelectWithWeightOrProbEntries(Entries, &RandomStream);
}

int32 USelectorUtils::SelectWithCumWeights(const TArray<double>& CumWeights, const FRandomStream* RandomStream)
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

	return SelectWithCumWeightsHelper(CumWeights, Num, SumWeight, RandomStream);
}

int32 USelectorUtils::SelectWithWeights(const TArray<double>& Weights, const FRandomStream* RandomStream)
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

	return SelectWithCumWeightsHelper(CumWeights, Num, SumWeight, RandomStream);
}

int32 USelectorUtils::SelectWithCumProbs(const TArray<double>& CumProbs, const FRandomStream* RandomStream)
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

	return SelectWithCumProbsHelper(CumProbs, Num, RandomStream);
}

int32 USelectorUtils::SelectWithProbs(const TArray<double>& Probs, const FRandomStream* RandomStream)
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

	return SelectWithCumProbsHelper(CumProbs, Num, RandomStream);
}

int32 USelectorUtils::SelectWithCookedDistribution(const FCookedSelectorDistribution& Distribution, const FRandomStream* RandomStream)
{
	if (Distribution.bIsProbs)
	{
		return SelectWithCumProbs(Distribution.CumWeightsOrCumProbs, RandomStream);
	}
	return SelectWithCumWeights(Distribution.CumWeightsOrCumProbs, RandomStream);
}

int32 USelectorUtils::SelectWithWeightOrProbEntries(const TArray<FWeightOrProbEntry>& Entries, const FRandomStream* RandomStream)
{
	int32 Num = Entries.Num();
	if (Num == 0)
	{
		return -1;
	}

	double SumWeight = 0.0;
	double SumProb = 0.0;
	for (int32 Idx = 0; Idx < Num; Idx++)
	{
		if (Entries[Idx].bIsProb)
		{
			SumProb += FMath::Max(Entries[Idx].WeightOrProb, 0.0);
		}
		else
		{
			SumWeight += FMath::Max(Entries[Idx].WeightOrProb, 0.0);
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
			if (!Entries[Idx].bIsProb)
			{
				SumWeight += FMath::Max(Entries[Idx].WeightOrProb, 0.0);
			}
			CumWeights[Idx] = SumWeight;
		}
		return SelectWithCumWeightsHelper(CumWeights, Num, SumWeight, RandomStream);
	}

	// pure probabilities
	if (SumWeight == 0.0 || 1.0 - SumProb < 1e-6)
	{
		SumProb = 0.0;
		TArray<double> CumProbs;
		CumProbs.SetNum(Num);
		for (int32 Idx = 0; Idx < Num; Idx++)
		{
			if (Entries[Idx].bIsProb)
			{
				SumProb += FMath::Max(Entries[Idx].WeightOrProb, 0.0);
			}
			CumProbs[Idx] = SumProb;
			if (Idx < Num - 1 && SumProb >= 1.0)  // cut off at cum prob of 1.0
			{
				Num = Idx + 1;
				CumProbs.SetNum(Num);
				break;
			}
		}
		return SelectWithCumProbsHelper(CumProbs, Num, RandomStream);
	}

	// mixture: convert to weights
	const double WeightFactor = SumWeight / (1.0 - SumProb);
	SumWeight = 0.0;
	TArray<double> CumWeights;
	CumWeights.SetNum(Num);
	for (int32 Idx = 0; Idx < Num; Idx++)
	{
		if (Entries[Idx].bIsProb)
		{
			SumWeight += FMath::Max(Entries[Idx].WeightOrProb, 0.0) * WeightFactor;
		}
		else
		{
			SumWeight += FMath::Max(Entries[Idx].WeightOrProb, 0.0);
		}
		CumWeights[Idx] = SumWeight;
	}
	return SelectWithCumWeightsHelper(CumWeights, Num, SumWeight, RandomStream);
}

int32 USelectorUtils::SelectWithCumWeightsHelper(const TArray<double>& CumWeights, const int32 Num, const double SumWeight, const FRandomStream* RandomStream)
{
	const double RandomRoll = UCommonUtils::FRandRangeMaybeWithStream(0.0, SumWeight, RandomStream);
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

int32 USelectorUtils::SelectWithCumProbsHelper(const TArray<double>& CumProbs, const int32 Num, const FRandomStream* RandomStream)
{
	const double RandomRoll = UCommonUtils::FRandMaybeWithStream(RandomStream);
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
