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
	/** Binary search for insertion index in an increasing array, with almost no safety check. */
	UFUNCTION(BlueprintPure)
	static int32 BinarySearchForInsertion(const double target_key, const TArray<double>& increasing_keys);

	/** Binary search for insertion index within specified range in an increasing array, with almost no safety check. */
	UFUNCTION(BlueprintPure)
	static int32 BinarySearchForInsertionInSegment(const double target_key, const TArray<double>& increasing_keys, const int32 start_index, const int32 end_index);
};
