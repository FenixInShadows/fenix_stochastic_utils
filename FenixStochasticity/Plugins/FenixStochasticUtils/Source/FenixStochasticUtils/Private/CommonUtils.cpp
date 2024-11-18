// Copyright 2024, Tiannan Chen, All rights reserved.


#include "CommonUtils.h"

int32 UCommonUtils::BinarySearchForInsertion(const double TargetKey, const TArray<double>& IncreasingKeys)
{
	return BinarySearchForInsertionInSegment(TargetKey, IncreasingKeys, 0, IncreasingKeys.Num());
}

int32 UCommonUtils::BinarySearchForInsertionInSegment(const double TargetKey, const TArray<double>& IncreasingKeys, const int32 StartIndex, const int32 EndIndex)
{
	// simple binary recursion converted to iteration
	int32 Start = StartIndex;
	int32 End = EndIndex;
	while (End > Start)
	{
		const int32 Mid = (Start + End) / 2;
		const double MidKey = IncreasingKeys[Mid];
		if (TargetKey < MidKey)
		{
			End = Mid;
		}
		else
		{
			Start = Mid + 1;
		}
	}
	return Start;
}

void UCommonUtils::MakeSimpleCumulatives(const TArray<double>& Values, TArray<double>& OutCumulatives)
{
	const int32 Num = Values.Num();

	OutCumulatives.SetNum(Num);
	double SumValue = 0.0;

	for (int32 Idx = 0; Idx < Num; Idx++)
	{
		SumValue += Values[Idx];
		OutCumulatives[Idx] = SumValue;
	}
}
