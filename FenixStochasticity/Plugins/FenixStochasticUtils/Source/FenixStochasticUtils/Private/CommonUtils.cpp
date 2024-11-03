// Copyright 2024, Tiannan Chen, All rights reserved.


#include "CommonUtils.h"

int32 UCommonUtils::BinarySearchForInsertion(double target_key, const TArray<double>& increasing_keys)
{
	return BinarySearchForInsertionInSegment(target_key, increasing_keys, 0, increasing_keys.Num());
}

int32 UCommonUtils::BinarySearchForInsertionInSegment(double target_key, const TArray<double>& increasing_keys, int32 start_index, int32 end_index)
{
	if (end_index <= start_index)
	{
		return start_index;
	}

	int32 mid_index = (start_index + end_index) / 2;
	float mid_key = increasing_keys[mid_index];
	if (target_key < mid_key)
	{
		return BinarySearchForInsertionInSegment(target_key, increasing_keys, start_index, mid_index);
	}
	return BinarySearchForInsertionInSegment(target_key, increasing_keys, mid_index + 1, end_index);
}
