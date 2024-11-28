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
struct FENIXSTOCHASTICUTILS_API FWeightOrProbEntry
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
struct FENIXSTOCHASTICUTILS_API FCookedSelectorDistribution
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
UCLASS(meta = (BlueprintThreadSafe))
class FENIXSTOCHASTICUTILS_API USelectorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	///** Create and return a RandomSelector. */
	//UFUNCTION(BlueprintCallable)
	//static URandomSelector* CreateRandomSelector(const FRandomSelectorConfig& Config);

public: // Blueprint and C++ APIs
	/**
	* Compute additive cumulation of values, which can be used for computing cumulative weights or probabilities.
	* Parameter ValueLowerClamp is used for clamping at the lower bounds of each of the values before being cumulated (e.g. regard weights or probabilities as zeros).
	* Best used on cases those original values do not change. Needs remake to update with the change on the original values.
	*/
	UFUNCTION(BlueprintPure, meta = (AdvancedDisplay = 2), Category = "Fenix|SelectorUtils|SelectionPreprocessing")
	static void MakeCumulatives(const TArray<double>& Values, TArray<double>& OutCumulatives, double ValueLowerClamp = 0.0);

	/**
	* Compute additive cumulation of values with a cutoff on the total, which can be used for computing cumulative probabilities.
	* Parameter ValueLowerClamp is used for clamping at the lower bounds of each of the values before being cumulated (e.g. regard weights or probabilities as zeros).
	* Parameter TotalCutoff is used for cutting off later entries when the earlier entries already cumulates to no less than this parameter (e.g. cut off cumulative probability at 1.0)
	* Due to possible cut off, the output array may be shorter than the input array. 
	* Best used on cases those original values do not change. Needs remake to update with the change on the original values.
	*/
	UFUNCTION(BlueprintPure, meta = (AdvancedDisplay = 2), Category = "Fenix|SelectorUtils|SelectionPreprocessing")
	static void MakeCumulativesWithCutoff(const TArray<double>& Values, TArray<double>& OutCumulatives, double ValueLowerClamp = 0.0, double TotalCutoff = 1.0);

	/**
	* Make CookedSelectorDistribution from an array of WeightOrProbEntry's.
	* Probabilities entries get their portion first then the remaining probabilities (if any) are considered for weight entries.
	* The result is represented as probabilities if there are no weight entries or the total from probability entries adds up to no less than 1.0, otherwise reprenented as weights.
	* Best used on cases where the WeightOrProbEntry's do not change. Needs remake when they get changed.
	*/
	UFUNCTION(BlueprintPure, Category = "Fenix|SelectorUtils|SelectionPreprocessing")
	static void CookSelectorDistribution(const TArray<FWeightOrProbEntry>& Entries, FCookedSelectorDistribution& OutDistribution);

	/** Get an array of FWeightOrProbEntry's from a data table. */
	UFUNCTION(BlueprintCallable, Category = "Fenix|SelectorUtils|DataTable")
	static void GetWeightOrProbEntriesFromDataTable(const UDataTable* DataTable, TArray<FWeightOrProbEntry>& OutEntries, FName IsProbPropertyName = "IsProb", FName WeightOrProbPropertyName = "WeightOrProb");

private: // Blueprint only APIs
	/**
	* Select index with given cumulative weights, negative returning value means failure. Not thread safe.
	* Require input non-negative and non-decreasing.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With Cum Weights", NotBlueprintThreadSafe), Category = "Fenix|SelectorUtils|Selection")
	static UPARAM(DisplayName = "Out Index") int32 BPFunc_SelectWithCumWeights(const TArray<double>& CumWeights);

	/**
	* Select index with given cumulative weights and a random stream (can be seeded), negative returning value means failure.
	* Require input non-negative and non-decreasing.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With Cum Weights From Stream"), Category = "Fenix|SelectorUtils|Selection")
	static UPARAM(DisplayName = "Out Index") int32 BPFunc_SelectWithCumWeightsFromStream(const TArray<double>& CumWeights, UPARAM(ref) FRandomStream& Stream);

	/** Select index with given weights, negative returning value means failure. Not thread safe.*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With Weights", NotBlueprintThreadSafe), Category = "Fenix|SelectorUtils|Selection")
	static UPARAM(DisplayName = "Out Index") int32 BPFunc_SelectWithWeights(const TArray<double>& Weights);

	/** Select index with given weights and a random stream (can be seeded), negative returning value means failure. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With Weights From Stream"), Category = "Fenix|SelectorUtils|Selection")
	static UPARAM(DisplayName = "Out Index") int32 BPFunc_SelectWithWeightsFromStream(const TArray<double>& Weights, UPARAM(ref) FRandomStream& Stream);

	/**
	* Select index with given cumulative probabilities, negative returning value means failure. Not thread safe.
	* Cut off or padded at the end to a cumulative probability of 1.0 if the total is more.
	* If the total is not enough, then when it rolls outside it counts as failure.
	* Require input non-negative and non-decreasing.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With Cum Probs", NotBlueprintThreadSafe), Category = "Fenix|SelectorUtils|Selection")
	static UPARAM(DisplayName = "Out Index") int32 BPFunc_SelectWithCumProbs(const TArray<double>& CumProbs);

	/**
	* Select index with given cumulative probabilities and a random stream, negative returning value means failure.
	* Cut off or padded at the end to a cumulative probability of 1.0 if the total is more.
	* If the total is not enough, then when it rolls outside it counts as failure.
	* Require input non-negative and non-decreasing.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With Cum Probs From Stream"), Category = "Fenix|SelectorUtils|Selection")
	static UPARAM(DisplayName = "Out Index") int32 BPFunc_SelectWithCumProbsFromStream(const TArray<double>& CumProbs, UPARAM(ref) FRandomStream& Stream);
	
	/**
	* Select index with given probabilities, negative returning value means failure. Not thread safe.
	* Cut off or padded at the end to a cumulative probability of 1.0 if the total is more.
	* If the total is not enough, then when it rolls outside it counts as failure.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With Probs", NotBlueprintThreadSafe), Category = "Fenix|SelectorUtils|Selection")
	static UPARAM(DisplayName = "Out Index") int32 BPFunc_SelectWithProbs(const TArray<double>& Probs);

	/**
	* Select index with given probabilities and a random stream, negative returning value means failure.
	* Cut off or padded at the end to a cumulative probability of 1.0 if the total is more.
	* If the total is not enough, then when it rolls outside it counts as failure.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With Probs From Stream"), Category = "Fenix|SelectorUtils|Selection")
	static UPARAM(DisplayName = "Out Index") int32 BPFunc_SelectWithProbsFromStream(const TArray<double>& Probs, UPARAM(ref) FRandomStream& Stream);

	/**
	* Select index with given CookedSelectorDistribution, negative returning value means failure. Not thread safe.
	* If the input is probabilities, then cut off to a cumulative probability of 1.0 if the total is more.
	* If the input is probabilities and and the total is not enough, then when it rolls outside it counts as failure.
	* Require input weights or probs non-negative and non-decreasing.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With CookedDistribution", NotBlueprintThreadSafe), Category = "Fenix|SelectorUtils|Selection")
	static UPARAM(DisplayName = "Out Index") int32 BPFunc_SelectWithCookedDistribution(const FCookedSelectorDistribution& Distribution);

	/**
	* Select index with given CookedSelectorDistribution and a random stream, negative returning value means failure.
	* If the input is probabilities, then cut off to a cumulative probability of 1.0 if the total is more.
	* If the input is probabilities and and the total is not enough, then when it rolls outside it counts as failure.
	* Require input weights or probs non-negative and non-decreasing.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With CookedDistribution From Stream"), Category = "Fenix|SelectorUtils|Selection")
	static UPARAM(DisplayName = "Out Index") int32 BPFunc_SelectWithCookedDistributionFromStream(const FCookedSelectorDistribution& Distribution, UPARAM(ref) FRandomStream& Stream);

	/**
	* Select index with given WeightOrProbEntry's, negative returning value means failure. Not thread safe.
	* Probabilities entries get their portion first then the remaining probabilities (if any) are considered for weight entries.
	* Probability entries are cut off at the end to a cumulative probability of 1.0 if the total is more.
	* If all positive entries are probabilities and the total is not enough, then when it rolls outside it counts as failure.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With WeightOrProbEntries", NotBlueprintThreadSafe), Category = "Fenix|SelectorUtils|Selection")
	static UPARAM(DisplayName = "Out Index") int32 BPFunc_SelectWithWeightOrProbEntries(const TArray<FWeightOrProbEntry>& Entries);

	/**
	* Select index with given WeightOrProbEntry's and a random stream, negative returning value means failure.
	* Probabilities entries get their portion first then the remaining probabilities (if any) are considered for weight entries.
	* Probability entries are cut off at the end to a cumulative probability of 1.0 if the total is more.
	* If all positive entries are probabilities and the total is not enough, then when it rolls outside it counts as failure.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Select With WeightOrProbEntries From Stream"), Category = "Fenix|SelectorUtils|Selection")
	static UPARAM(DisplayName = "Out Index") int32 BPFunc_SelectWithWeightOrProbEntriesFromStream(const TArray<FWeightOrProbEntry>& Entries, UPARAM(ref) FRandomStream& Stream);

public:  // C++ only APIs
	/**
	* Select index with given cumulative weights, negative returning value means failure.
	* Require input non-negative and non-decreasing.
	* Using a random stream if the optional input Stream is not nullptr. Threadsafe only when using a stream.
	*/
	static int32 SelectWithCumWeights(const TArray<double>& CumWeights, FRandomStream* Stream = nullptr);

	/** 
	* Select index with given weights, negative returning value means failure.
	* Using a random stream if the optional input Stream is not nullptr. Threadsafe only when using a stream.
	*/
	static int32 SelectWithWeights(const TArray<double>& Weights, FRandomStream* Stream = nullptr);

	/**
	* Select index with given cumulative probabilities, negative returning value means failure.
	* Cut off or padded at the end to a cumulative probability of 1.0 if the total is more.
	* If the total is not enough, then when it rolls outside it counts as failure.
	* Require input non-negative and non-decreasing.
	* Using a random stream if the optional input Stream is not nullptr. Threadsafe only when using a stream.
	*/
	static int32 SelectWithCumProbs(const TArray<double>& CumProbs, FRandomStream* Stream = nullptr);
	
	/**
	* Select index with given probabilities, negative returning value means failure.
	* Cut off or padded at the end to a cumulative probability of 1.0 if the total is more.
	* If the total is not enough, then when it rolls outside it counts as failure.
	* Using a random stream if the optional input Stream is not nullptr. Threadsafe only when using a stream.
	*/
	static int32 SelectWithProbs(const TArray<double>& Probs, FRandomStream* Stream = nullptr);

	/**
	* Select index with given CookedSelectorDistribution, negative returning value means failure.
	* If the input is probabilities, then cut off to a cumulative probability of 1.0 if the total is more. 
	* If the input is probabilities and and the total is not enough, then when it rolls outside it counts as failure.
	* Require input weights or probs non-negative and non-decreasing.
	* Using a random stream if the optional input Stream is not nullptr. Threadsafe only when using a stream.
	*/
	static int32 SelectWithCookedDistribution(const FCookedSelectorDistribution& Distribution, FRandomStream* Stream = nullptr);

	/**
	* Select index with given WeightOrProbEntry's, negative returning value means failure.
	* Probabilities entries get their portion first then the remaining probabilities (if any) are considered for weight entries.
	* Probability entries are cut off at the end to a cumulative probability of 1.0 if the total is more.
	* If all positive entries are probabilities and the total is not enough, then when it rolls outside it counts as failure.
	* Using a random stream if the optional input Stream is not nullptr. Threadsafe only when using a stream.
	*/
	static int32 SelectWithWeightOrProbEntries(const TArray<FWeightOrProbEntry>& Entries, FRandomStream* Stream = nullptr);

private:
	/** 
	* Helper for selecting with weights. It assumes Num and SumWeight being appropriate and non-zero.
	* Using a random stream if the optional input Stream is not nullptr. Threadsafe only when using a stream.
	*/
	static int32 SelectWithCumWeightsHelper(const TArray<double>& CumWeights, const int32 Num, const double SumWeight, FRandomStream* Stream = nullptr);
	
	/** 
	* Helper for selecting with probabilities. It assumes Num being appropriate and non-zero.
	* Using a random stream if the optional input Stream is not nullptr. Threadsafe only when using a stream.
	*/
	static int32 SelectWithCumProbsHelper(const TArray<double>& CumProbs, const int32 Num, FRandomStream* Stream = nullptr);
};
