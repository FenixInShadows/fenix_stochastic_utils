// Copyright 2024, Tiannan Chen, All rights reserved.


#include "CommonUtils.h"

int32 UCommonUtils::BinarySearchForInsertion(const double target_key, const TArray<double>& increasing_keys)
{
	return BinarySearchForInsertionInSegment(target_key, increasing_keys, 0, increasing_keys.Num());
}

int32 UCommonUtils::BinarySearchForInsertionInSegment(const double target_key, const TArray<double>& increasing_keys, const int32 start_index, const int32 end_index)
{
	// simple binary recursion converted to iteration
	int32 start = start_index;
	int32 end = end_index;
	while (end > start)
	{
		int32 mid = (start + end) / 2;
		float mid_key = increasing_keys[mid];
		if (target_key < mid_key)
		{
			end = mid;
		}
		else
		{
			start = mid + 1;
		}
	}
	return start;
}
