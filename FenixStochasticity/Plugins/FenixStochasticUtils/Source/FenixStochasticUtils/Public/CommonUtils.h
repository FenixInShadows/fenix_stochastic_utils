// Copyright 2025, Tiannan Chen, All rights reserved.

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
	UFUNCTION(BlueprintPure, Category = "Fenix|CommonUtils|Search")
	static UPARAM(DisplayName = "Out Index") int32 BinarySearchForInsertion(const double TargetKey, const TArray<double>& IncreasingKeys);

	/** Binary search for insertion index within specified range in an increasing array, with almost no safety check. */
	UFUNCTION(BlueprintPure, Category = "Fenix|CommonUtils|Search")
	static UPARAM(DisplayName = "Out Index") int32 BinarySearchForInsertionInSegment(const double TargetKey, const TArray<double>& IncreasingKeys, const int32 StartIndex, const int32 EndIndex);

	/** Compute simple additive cumulation of values in an array, outputing the cumulative array. */
	UFUNCTION(BlueprintCallable, Category = "Fenix|CommonUtils|ArrayMath")
	static void MakeSimpleCumulatives(const TArray<double>& Values, TArray<double>& OutCumulatives);

	/** Get data table column as a floating point value array. */
	UFUNCTION(BlueprintCallable, Category = "Fenix|CommonUtils|DataTable")
	static void GetDataTableColumnAsFloats(const UDataTable* DataTable, FName PropertyName, TArray<double>& OutValues);

	/** The InContainer variant of GetFloatingPointPropertyValue from the engine. */
	static FORCEINLINE double GetFloatingPointPropertyValue_InContainer(const FNumericProperty* NumericProperty, const void* Container, const int32 ArrayIndex = 0)
	{
		return NumericProperty->GetFloatingPointPropertyValue(NumericProperty->ContainerPtrToValuePtr<void>(Container, ArrayIndex));
	}

	/** Get data table column as an integer value array. */
	UFUNCTION(BlueprintCallable, Category = "Fenix|CommonUtils|DataTable")
	static void GetDataTableColumnAsInts(const UDataTable* DataTable, FName PropertyName, TArray<int32>& OutValues);

	/** Get data table column as a bool value array. */
	UFUNCTION(BlueprintCallable, Category = "Fenix|CommonUtils|DataTable")
	static void GetDataTableColumnAsBools(const UDataTable* DataTable, FName PropertyName, TArray<bool>& OutValues);

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
