// Copyright 2024, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonUtils.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintThreadSafe))
class FENIXSTOCHASTICUTILS_API UCommonUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Binary search for insertion index in an increasing array, with almost no safety check. */
	UFUNCTION(BlueprintPure)
	static UPARAM(DisplayName = "Return Index") int32 BinarySearchForInsertion(const double TargetKey, const TArray<double>& IncreasingKeys);

	/** Binary search for insertion index within specified range in an increasing array, with almost no safety check. */
	UFUNCTION(BlueprintPure)
	static UPARAM(DisplayName = "Return Index") int32 BinarySearchForInsertionInSegment(const double TargetKey, const TArray<double>& IncreasingKeys, const int32 StartIndex, const int32 EndIndex);



	/** FRand logic, but using a random stream if the optional input Stream is not nullptr. Threadsafe only when using a stream. */
	static FORCEINLINE float FRandMaybeWithStream(FRandomStream* Stream = nullptr)
	{
		return Stream ? Stream->FRand() : FMath::FRand();
	}

	/** FRandRange logic, but using a random stream if the optional input Stream is not nullptr. Threadsafe only when using a stream. */
	static FORCEINLINE float FRandRangeMaybeWithStream(float InMin, float InMax, FRandomStream* Stream = nullptr)
	{
		return Stream ? Stream->FRandRange(InMin, InMax) : FMath::FRandRange(InMin, InMax);
	}

	/** FRandRange logic, but using a random stream if the optional input Stream is not nullptr. Threadsafe only when using a stream. */
	static FORCEINLINE double FRandRangeMaybeWithStream(double InMin, double InMax, FRandomStream* Stream = nullptr)
	{
		return Stream ? Stream->FRandRange(InMin, InMax) : FMath::FRandRange(InMin, InMax);
	}
};
