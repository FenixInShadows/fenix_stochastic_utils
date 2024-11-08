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

public:
	/** Create and return a RandomSelector. */
	UFUNCTION(BlueprintCallable)
	static URandomSelector* CreateRandomSelector(const FRandomSelectorConfig& config);

	/** Compute cumulative weights from weights, negative weights are regarded as zeros. */
	UFUNCTION(BlueprintPure)
	static TArray<double> MakeCumWeights(const TArray<double>& weights);

	/**
	* Select index with given cumulative weights, negative returning value means failure.
	* Require input non-negative and non-decreasing.
	*/
	UFUNCTION(BlueprintCallable)
	static int32 SelectWithCumWeights(const TArray<double>& cum_weights);

	/** Select index with given weights, negative returning value means failure. */
	UFUNCTION(BlueprintCallable)
	static int32 SelectWithWeights(const TArray<double>& weights);

	/**
	* Compute cumulative probabilities from probabilities, negative probabilities are regarded as zeros.
	* Cut off at the end to a cumulative probability of 1.0 (output array may be shorter).
	*/
	UFUNCTION(BlueprintPure)
	static TArray<double> MakeCumProbs(const TArray<double>& probs);

	/**
	* Select index with given cumulative probabilities, negative returning value means failure.
	* Cut off or padded at the end to a cumulative probability of 1.0.
	* Padding only happens at the last non-zero probability entry if total is not sufficient.
	* Require input non-negative and non-decreasing.
	*/
	UFUNCTION(BlueprintCallable)
	static int32 SelectWithCumProbs(const TArray<double>& cum_probs);
	
	/**
	* Select index with given probabilities, negative returning value means failure.
	* Cut off or padded at the end to a cumulative probability of 1.0.
	* Padding only happens at the last non-zero probability entry if total is not sufficient.
	*/
	UFUNCTION(BlueprintCallable)
	static int32 SelectWithProbs(const TArray<double>& probs);

	/** Make FCookedWeightsOrProbsConfig from an array of FWeightOrProbConfig's */
	UFUNCTION(BlueprintPure)
	static FCookedWeightsOrProbsConfig CookWeightOrProbConfigs(const TArray<FWeightOrProbConfig>& raw_configs);

	/**
	* Select index with given CookedWeightsOrProbsConfig, negative returning value means failure.
	* If the input is probabilities, then cut off or padded at the end to a cumulative probability of 1.0. 
	* Padding only happens at the last non-zero probability entry if total is not sufficient.
	* Require input weights or probs non-negative and non-decreasing.
	*/
	UFUNCTION(BlueprintCallable)
	static int32 SelectWithCookedWeightsOrProbsConfig(const FCookedWeightsOrProbsConfig& cooked_config);

	/**
	* Select index with given WeightOrProbConfig's, negative returning value means failure.
	* Probabilities entries get priorities and the remaining probabilities (if any) are considered for weight entries.
	* Probability entries are cut off at the end to a cumulative probability of 1.0.
	* If all positive entries are probabilities, then it is also padded at the last non-zero probability entry to a cumulative probability of 1.0 if not sufficient.
	*/
	UFUNCTION(BlueprintCallable)
	static int32 SelectWithWeightOrProbConfigs(const TArray<FWeightOrProbConfig>& raw_configs);

private:
	static int32 SelectWithCumWeightsHelper(const TArray<double>& cum_weights, const int32 num, const double sum_weight);
	static int32 SelectWithCumProbsHelper(const TArray<double>& cum_probs, const int32 num);
};
