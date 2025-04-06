// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetArrayLibrary.h"

#include "CommonUtils.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintThreadSafe))
class FENIXSTOCHASTICUTILS_API UCommonUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Impure version of Array_Get */
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (ArrayParm = "TargetArray", ArrayTypeDependentParams = "Item", BlueprintThreadSafe), Category = "Fenix|CommonUtils|Array")
	static void Array_Get_Impure(const TArray<int32>& TargetArray, const int32 Index, int32& Item);
	DECLARE_FUNCTION(execArray_Get_Impure)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FArrayProperty>(NULL);
		void* ArrayAddr = Stack.MostRecentPropertyAddress;
		FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}
		P_GET_PROPERTY(FIntProperty, Index);

		// Since Item isn't really an int, step the stack manually
		const FProperty* InnerProp = ArrayProperty->Inner;
		const int32 PropertySize = InnerProp->ElementSize * InnerProp->ArrayDim;
		void* StorageSpace = FMemory_Alloca(PropertySize);
		InnerProp->InitializeValue(StorageSpace);

		Stack.MostRecentPropertyAddress = nullptr;
		Stack.MostRecentPropertyContainer = nullptr;
		Stack.StepCompiledIn<FProperty>(StorageSpace);
		const FFieldClass* InnerPropClass = InnerProp->GetClass();
		const FFieldClass* MostRecentPropClass = Stack.MostRecentProperty->GetClass();
		void* ItemPtr;
		// If the destination and the inner type are identical in size and their field classes derive from one another, then permit the writing out of the array element to the destination memory
		if (Stack.MostRecentPropertyAddress != NULL && (PropertySize == Stack.MostRecentProperty->ElementSize * Stack.MostRecentProperty->ArrayDim) &&
			(MostRecentPropClass->IsChildOf(InnerPropClass) || InnerPropClass->IsChildOf(MostRecentPropClass)))
		{
			ItemPtr = Stack.MostRecentPropertyAddress;
		}
		else
		{
			ItemPtr = StorageSpace;
		}

		P_FINISH;
		P_NATIVE_BEGIN;
		UKismetArrayLibrary::GenericArray_Get(ArrayAddr, ArrayProperty, Index, ItemPtr);
		P_NATIVE_END;
		InnerProp->DestroyValue(StorageSpace);
	}

	/** Get data table column as a floating point value array. */
	UFUNCTION(BlueprintCallable, Category = "Fenix|CommonUtils|DataTable")
	static void GetDataTableColumnAsFloats(const UDataTable* DataTable, const FName PropertyName, TArray<double>& OutValues);

	/** The InContainer variant of GetFloatingPointPropertyValue from the engine. */
	static FORCEINLINE double GetFloatingPointPropertyValue_InContainer(const FNumericProperty* NumericProperty, const void* Container, const int32 ArrayIndex = 0)
	{
		return NumericProperty->GetFloatingPointPropertyValue(NumericProperty->ContainerPtrToValuePtr<void>(Container, ArrayIndex));
	}

	/** Get data table column as an integer value array. */
	UFUNCTION(BlueprintCallable, Category = "Fenix|CommonUtils|DataTable")
	static void GetDataTableColumnAsInts(const UDataTable* DataTable, const FName PropertyName, TArray<int32>& OutValues);

	/** Get data table column as a bool value array. */
	UFUNCTION(BlueprintCallable, Category = "Fenix|CommonUtils|DataTable")
	static void GetDataTableColumnAsBools(const UDataTable* DataTable, const FName PropertyName, TArray<bool>& OutValues);

	/** Binary search for insertion index in an increasing array, with almost no safety check. */
	UFUNCTION(BlueprintPure, Category = "Fenix|CommonUtils|Search")
	static UPARAM(DisplayName = "OutIndex") int32 BinarySearchForInsertion(const double TargetKey, const TArray<double>& IncreasingKeys);

	/** Binary search for insertion index within specified range in an increasing array, with almost no safety check. */
	UFUNCTION(BlueprintPure, Category = "Fenix|CommonUtils|Search")
	static UPARAM(DisplayName = "OutIndex") int32 BinarySearchForInsertionInSegment(const double TargetKey, const TArray<double>& IncreasingKeys, const int32 StartIndex, const int32 EndIndex);

	/** Compute simple additive cumulation of values in an array, outputing the cumulative array. */
	UFUNCTION(BlueprintCallable, Category = "Fenix|CommonUtils|ArrayMath")
	static void MakeSimpleCumulatives(const TArray<double>& Values, TArray<double>& OutCumulatives);

	/** FRand logic, but using a random stream if the optional input RandomStream is not nullptr. Threadsafe only when using a stream. */
	static FORCEINLINE float FRandMaybeWithStream(const FRandomStream* RandomStream = nullptr)
	{
		return RandomStream ? RandomStream->FRand() : FMath::FRand();
	}

	/** FRandRange logic, but using a random stream if the optional input RandomStream is not nullptr. Threadsafe only when using a stream. */
	static FORCEINLINE float FRandRangeMaybeWithStream(const float InMin, const float InMax, const FRandomStream* RandomStream = nullptr)
	{
		return RandomStream ? RandomStream->FRandRange(InMin, InMax) : FMath::FRandRange(InMin, InMax);
	}

	/** FRandRange logic, but using a random stream if the optional input RandomStream is not nullptr. Threadsafe only when using a stream. */
	static FORCEINLINE double FRandRangeMaybeWithStream(const double InMin, const double InMax, const FRandomStream* RandomStream = nullptr)
	{
		return RandomStream ? RandomStream->FRandRange(InMin, InMax) : FMath::FRandRange(InMin, InMax);
	}

	/** Platform time can be used to measure execution time. */
	UFUNCTION(BlueprintCallable, Category = "Fenix|CommonUtils|Timing")
	static double GetCurrentPlatformTime();

	UFUNCTION(BlueprintCallable)
	static void Step1(const TArray<double>& TestArray, TArray<double>& OutArray);

	UFUNCTION(BlueprintCallable)
	static void Step2(const TArray<double>& TestArray, TArray<double>& OutArray);

	UFUNCTION(BlueprintCallable)
	static void Step3(const TArray<double>& TestArray, TArray<double>& OutArray);

	UFUNCTION(BlueprintCallable)
	static void CombinedSteps(const TArray<double>& TestArray, TArray<double>& OutArray);

	static void MakeSimpleCumulativesRaw(const TArray<double>& Values, TArray<double>& OutCumulatives);
	static void Step1Raw(const TArray<double>& TestArray, TArray<double>& OutArray);
	static void Step2Raw(const TArray<double>& TestArray, TArray<double>& OutArray);
	static void Step3Raw(const TArray<double>& TestArray, TArray<double>& OutArray);

	UFUNCTION(BlueprintCallable)
	static void CombinedStepsRaw(const TArray<double>& TestArray, TArray<double>& OutArray);
};
