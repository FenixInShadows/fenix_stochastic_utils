// Copyright 2025, Tiannan Chen, All rights reserved.


#include "CommonUtils.h"
#include "Engine/DataTable.h"

int32 UCommonUtils::BinarySearchForInsertion(const double TargetKey, const TArray<double>& IncreasingKeys)
{
	return BinarySearchForInsertionInSegment(TargetKey, IncreasingKeys, 0, IncreasingKeys.Num());
}

void UCommonUtils::GetDataTableColumnAsFloats(const UDataTable* DataTable, const FName PropertyName, TArray<double>& OutValues)
{
	OutValues.Empty();
	if (DataTable && PropertyName != NAME_None)
	{
		const FProperty* ColumnProperty = DataTable->FindTableProperty(PropertyName);
		if (ColumnProperty)
		{
			if (const FNumericProperty* NumericProperty = CastField<FNumericProperty>(ColumnProperty))
			{
				for (auto RowIt = DataTable->GetRowMap().CreateConstIterator(); RowIt; ++RowIt)
				{
					uint8* RowData = RowIt.Value();
					OutValues.Add(GetFloatingPointPropertyValue_InContainer(NumericProperty, RowData));
				}
			}
		}
	}
}

void UCommonUtils::GetDataTableColumnAsInts(const UDataTable* DataTable, const FName PropertyName, TArray<int32>& OutValues)
{
	OutValues.Empty();
	if (DataTable && PropertyName != NAME_None)
	{
		const FProperty* ColumnProperty = DataTable->FindTableProperty(PropertyName);
		if (ColumnProperty)
		{
			if (const FNumericProperty* NumericProperty = CastField<FNumericProperty>(ColumnProperty))
			{
				for (auto RowIt = DataTable->GetRowMap().CreateConstIterator(); RowIt; ++RowIt)
				{
					uint8* RowData = RowIt.Value();
					OutValues.Add(NumericProperty->GetSignedIntPropertyValue_InContainer(RowData));
				}
			}
		}
	}
}

void UCommonUtils::GetDataTableColumnAsBools(const UDataTable* DataTable, const FName PropertyName, TArray<bool>& OutValues)
{
	OutValues.Empty();
	if (DataTable && PropertyName != NAME_None)
	{
		const FProperty* ColumnProperty = DataTable->FindTableProperty(PropertyName);
		if (ColumnProperty)
		{
			if (const FBoolProperty* BoolProperty = CastField<FBoolProperty>(ColumnProperty))
			{
				for (auto RowIt = DataTable->GetRowMap().CreateConstIterator(); RowIt; ++RowIt)
				{
					uint8* RowData = RowIt.Value();
					OutValues.Add(BoolProperty->GetPropertyValue_InContainer(RowData));
				}
			}
		}
	}
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

double UCommonUtils::GetCurrentPlatformTime()
{
	return FPlatformTime::Seconds();
}

void UCommonUtils::Step1(const TArray<double>& TestArray, TArray<double>& OutArray)
{
	MakeSimpleCumulatives(TestArray, OutArray);
}

void UCommonUtils::Step2(const TArray<double>& TestArray, TArray<double>& OutArray)
{
	MakeSimpleCumulatives(TestArray, OutArray);
}

void UCommonUtils::Step3(const TArray<double>& TestArray, TArray<double>& OutArray)
{
	MakeSimpleCumulatives(TestArray, OutArray);
}

void UCommonUtils::CombinedSteps(const TArray<double>& TestArray, TArray<double>& OutArray)
{
	Step1(TestArray, OutArray);
	Step2(TestArray, OutArray);
	Step3(TestArray, OutArray);
}

void UCommonUtils::MakeSimpleCumulativesRaw(const TArray<double>& Values, TArray<double>& OutCumulatives)
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

void UCommonUtils::Step1Raw(const TArray<double>& TestArray, TArray<double>& OutArray)
{
	MakeSimpleCumulativesRaw(TestArray, OutArray);
}

void UCommonUtils::Step2Raw(const TArray<double>& TestArray, TArray<double>& OutArray)
{
	MakeSimpleCumulativesRaw(TestArray, OutArray);
}

void UCommonUtils::Step3Raw(const TArray<double>& TestArray, TArray<double>& OutArray)
{
	MakeSimpleCumulativesRaw(TestArray, OutArray);
}

void UCommonUtils::CombinedStepsRaw(const TArray<double>& TestArray, TArray<double>& OutArray)
{
	Step1Raw(TestArray, OutArray);
	Step2Raw(TestArray, OutArray);
	Step3Raw(TestArray, OutArray);
}

