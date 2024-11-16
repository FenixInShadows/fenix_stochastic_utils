// Copyright 2024, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "RandomSelector.h"

#include "SelectorUtils.generated.h"

/** 
* A config recording a weight or probability (also recording which type it is).
*/
USTRUCT(BlueprintType)
struct FENIXSTOCHASTICUTILS_API FWeightOrProbConfig
{
	GENERATED_BODY()

	/** Whether it records probability (as opposed to weight). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsProb = false;

	/** Weight or probability value. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double WeightOrProb = 1.0;
};

/**
* A config recording a cumulative weight array or cumulative probability array (also recording which type it is).
*/
USTRUCT(BlueprintType)
struct FENIXSTOCHASTICUTILS_API FCookedWeightsOrProbsConfig
{
	GENERATED_BODY()

	/** Whether it records probabilities (as opposed to weights). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsProbs = false;

	/** Weight or probability array. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<double> CumWeightsOrCumProbs = { 1.0 };
};

/**
 * 
 */
UCLASS()
class FENIXSTOCHASTICUTILS_API USelectorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	///** Create and return a RandomSelector. */
	//UFUNCTION(BlueprintCallable)
	//static URandomSelector* CreateRandomSelector(const FRandomSelectorConfig& Config);

public:
	/** Compute cumulative weights from weights, negative weights are regarded as zeros. */
	UFUNCTION(BlueprintPure)
	static TArray<double> MakeCumWeights(const TArray<double>& Weights);

	/**
	* Compute cumulative probabilities from probabilities, negative probabilities are regarded as zeros.
	* Cut off at the end to a cumulative probability of 1.0 (output array may be shorter).
	*/
	UFUNCTION(BlueprintPure)
	static TArray<double> MakeCumProbs(const TArray<double>& Probs);

	/**
	* Make FCookedWeightsOrProbsConfig from an array of FWeightOrProbConfig's.
	* Probabilities entries get their portion first then the remaining probabilities (if any) are considered for weight entries.
	* Probability entries are cut off at the end to a cumulative probability of 1.0.
	* If all positive entries are probabilities, then it is also padded at the last non-zero probability entry to a cumulative probability of 1.0 if not sufficient.
	*/
	UFUNCTION(BlueprintPure)
	static FCookedWeightsOrProbsConfig CookWeightOrProbConfigs(const TArray<FWeightOrProbConfig>& RawConfigs);

private:
	/**
	* Select index with given cumulative weights, negative returning value means failure.
	* Require input non-negative and non-decreasing.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With Cum Weights"))
	static int32 BPFunc_SelectWithCumWeights(const TArray<double>& CumWeights);

	/**
	* Select index with given cumulative weights and a random stream (can be seeded), negative returning value means failure.
	* Require input non-negative and non-decreasing.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With Cum Weights From Stream"))
	static int32 BPFunc_SelectWithCumWeightsFromStream(const TArray<double>& CumWeights, UPARAM(ref) FRandomStream& Stream);

	/** Select index with given weights, negative returning value means failure. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With Weights"))
	static int32 BPFunc_SelectWithWeights(const TArray<double>& Weights);

	/** Select index with given weights and a random stream (can be seeded), negative returning value means failure. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With Weights From Stream"))
	static int32 BPFunc_SelectWithWeightsFromStream(const TArray<double>& Weights, UPARAM(ref) FRandomStream& Stream);

	/**
	* Select index with given cumulative probabilities, negative returning value means failure.
	* Cut off or padded at the end to a cumulative probability of 1.0 if the total is more.
	* If the total is not enough, then when it rolls outside it counts as failure.
	* Require input non-negative and non-decreasing.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With Cum Probs"))
	static int32 BPFunc_SelectWithCumProbs(const TArray<double>& CumProbs);

	/**
	* Select index with given cumulative probabilities and a random stream, negative returning value means failure.
	* Cut off or padded at the end to a cumulative probability of 1.0 if the total is more.
	* If the total is not enough, then when it rolls outside it counts as failure.
	* Require input non-negative and non-decreasing.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With Cum Probs From Stream"))
	static int32 BPFunc_SelectWithCumProbsFromStream(const TArray<double>& CumProbs, UPARAM(ref) FRandomStream& Stream);
	
	/**
	* Select index with given probabilities, negative returning value means failure.
	* Cut off or padded at the end to a cumulative probability of 1.0 if the total is more.
	* If the total is not enough, then when it rolls outside it counts as failure.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With Probs"))
	static int32 BPFunc_SelectWithProbs(const TArray<double>& Probs);

	/**
	* Select index with given probabilities and a random stream, negative returning value means failure.
	* Cut off or padded at the end to a cumulative probability of 1.0 if the total is more.
	* If the total is not enough, then when it rolls outside it counts as failure.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With Probs From Stream"))
	static int32 BPFunc_SelectWithProbsFromStream(const TArray<double>& Probs, UPARAM(ref) FRandomStream& Stream);

	/**
	* Select index with given CookedWeightsOrProbsConfig, negative returning value means failure.
	* If the input is probabilities, then cut off to a cumulative probability of 1.0 if the total is more.
	* If the input is probabilities and and the total is not enough, then when it rolls outside it counts as failure.
	* Require input weights or probs non-negative and non-decreasing.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With CookedWeightsOrProbsConfig"))
	static int32 BPFunc_SelectWithCookedWeightsOrProbsConfig(const FCookedWeightsOrProbsConfig& CookedConfig);

	/**
	* Select index with given CookedWeightsOrProbsConfig and a random stream, negative returning value means failure.
	* If the input is probabilities, then cut off to a cumulative probability of 1.0 if the total is more.
	* If the input is probabilities and and the total is not enough, then when it rolls outside it counts as failure.
	* Require input weights or probs non-negative and non-decreasing.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With CookedWeightsOrProbsConfig From Stream"))
	static int32 BPFunc_SelectWithCookedWeightsOrProbsConfigFromStream(const FCookedWeightsOrProbsConfig& CookedConfig, UPARAM(ref) FRandomStream& Stream);

	/**
	* Select index with given WeightOrProbConfig's, negative returning value means failure.
	* Probabilities entries get their portion first then the remaining probabilities (if any) are considered for weight entries.
	* Probability entries are cut off at the end to a cumulative probability of 1.0 if the total is more.
	* If all positive entries are probabilities and the total is not enough, then when it rolls outside it counts as failure.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With WeightOrProbConfigs"))
	static int32 BPFunc_SelectWithWeightOrProbConfigs(const TArray<FWeightOrProbConfig>& RawConfigs);

	/**
	* Select index with given WeightOrProbConfig's and a random stream, negative returning value means failure.
	* Probabilities entries get their portion first then the remaining probabilities (if any) are considered for weight entries.
	* Probability entries are cut off at the end to a cumulative probability of 1.0 if the total is more.
	* If all positive entries are probabilities and the total is not enough, then when it rolls outside it counts as failure.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With WeightOrProbConfigs From Stream"))
	static int32 BPFunc_SelectWithWeightOrProbConfigsFromStream(const TArray<FWeightOrProbConfig>& RawConfigs, UPARAM(ref) FRandomStream& Stream);

public:
	/**
	* Select index with given cumulative weights, negative returning value means failure.
	* Require input non-negative and non-decreasing.
	* Using a random stream if the optional input Stream is not nullptr.
	*/
	static int32 SelectWithCumWeights(const TArray<double>& CumWeights, FRandomStream* Stream = nullptr);

	/** 
	* Select index with given weights, negative returning value means failure.
	* Using a random stream if the optional input Stream is not nullptr.
	*/
	static int32 SelectWithWeights(const TArray<double>& Weights, FRandomStream* Stream = nullptr);

	/**
	* Select index with given cumulative probabilities, negative returning value means failure.
	* Cut off or padded at the end to a cumulative probability of 1.0 if the total is more.
	* If the total is not enough, then when it rolls outside it counts as failure.
	* Require input non-negative and non-decreasing.
	* Using a random stream if the optional input Stream is not nullptr.
	*/
	static int32 SelectWithCumProbs(const TArray<double>& CumProbs, FRandomStream* Stream = nullptr);
	
	/**
	* Select index with given probabilities, negative returning value means failure.
	* Cut off or padded at the end to a cumulative probability of 1.0 if the total is more.
	* If the total is not enough, then when it rolls outside it counts as failure.
	* Using a random stream if the optional input Stream is not nullptr.
	*/
	static int32 SelectWithProbs(const TArray<double>& Probs, FRandomStream* Stream = nullptr);

	/**
	* Select index with given CookedWeightsOrProbsConfig, negative returning value means failure.
	* If the input is probabilities, then cut off to a cumulative probability of 1.0 if the total is more. 
	* If the input is probabilities and and the total is not enough, then when it rolls outside it counts as failure.
	* Require input weights or probs non-negative and non-decreasing.
	* Using a random stream if the optional input Stream is not nullptr.
	*/
	static int32 SelectWithCookedWeightsOrProbsConfig(const FCookedWeightsOrProbsConfig& CookedConfig, FRandomStream* Stream = nullptr);

	/**
	* Select index with given WeightOrProbConfig's, negative returning value means failure.
	* Probabilities entries get their portion first then the remaining probabilities (if any) are considered for weight entries.
	* Probability entries are cut off at the end to a cumulative probability of 1.0 if the total is more.
	* If all positive entries are probabilities and the total is not enough, then when it rolls outside it counts as failure.
	* Using a random stream if the optional input Stream is not nullptr.
	*/
	static int32 SelectWithWeightOrProbConfigs(const TArray<FWeightOrProbConfig>& RawConfigs, FRandomStream* Stream = nullptr);


	UFUNCTION(BlueprintPure, CustomThunk, meta = (DisplayName = "Middle Array Item", CompactNodeTitle = "MID", ArrayParm = "TargetArray", ArrayTypeDependentParams = "Item"))
	static void GetMiddleItem(const TArray<int32>& TargetArray, int32& Item);

	DECLARE_FUNCTION(execGetMiddleItem)
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

		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FProperty>(nullptr);
		void* Result = Stack.MostRecentPropertyAddress;

		P_FINISH;
		P_NATIVE_BEGIN;
		Generic_GetMiddleItem(ArrayAddr, ArrayProperty, Result);
		P_NATIVE_END;
	}

	static void Generic_GetMiddleItem(void* TargetArray, const FArrayProperty* ArrayProp, void* OutItem);

private:
	/** Helper for selecting with weights. Using a random stream if the optional input Stream is not nullptr. It assumes Num and SumWeight being appropriate and non-zero. */
	static int32 SelectWithCumWeightsHelper(const TArray<double>& CumWeights, const int32 Num, const double SumWeight, FRandomStream* RandStream = nullptr);
	
	/** Helper for selecting with probabilities. Using a random stream if the optional input Stream is not nullptr. It assumes Num being appropriate and non-zero. */
	static int32 SelectWithCumProbsHelper(const TArray<double>& CumProbs, const int32 Num, FRandomStream* RandStream = nullptr);
};
