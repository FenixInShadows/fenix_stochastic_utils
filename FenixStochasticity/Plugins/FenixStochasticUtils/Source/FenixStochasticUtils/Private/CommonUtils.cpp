// Copyright 2024, Tiannan Chen, All rights reserved.


#include "CommonUtils.h"
#include "Engine/DataTable.h"

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

void UCommonUtils::GetDataTableColumnAsFloats(const UDataTable* DataTable, FName PropertyName, TArray<double>& OutValues)
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

void UCommonUtils::GetDataTableColumnAsInts(const UDataTable* DataTable, FName PropertyName, TArray<int32>& OutValues)
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

void UCommonUtils::GetDataTableColumnAsBools(const UDataTable* DataTable, FName PropertyName, TArray<bool>& OutValues)
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
