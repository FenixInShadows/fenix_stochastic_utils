// Copyright 2024, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonUtils.generated.h"

/**
 * 
 */
UCLASS()
class FENIXSTOCHASTICUTILS_API UCommonUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (ToolTip = "Binary search for insertion index in an increasing array, with almost no safety check."))
	static int32 BinarySearch(double target_key, const TArray<double>& increasing_keys);

	UFUNCTION(BlueprintPure, meta = (ToolTip = "Binary search for insertion index within specified range in an increasing array, with almost no safety check."))
	static int32 BinarySearchInSegment(double target_key, const TArray<double>& increasing_keys, int32 start_index, int32 end_index);
};
