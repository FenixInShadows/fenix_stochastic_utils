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
	static int32 BinarySearchForInsertion(const double TargetKey, const TArray<double>& IncreasingKeys);

	/** Binary search for insertion index within specified range in an increasing array, with almost no safety check. */
	UFUNCTION(BlueprintPure)
	static int32 BinarySearchForInsertionInSegment(const double TargetKey, const TArray<double>& IncreasingKeys, const int32 StartIndex, const int32 EndIndex);
};
