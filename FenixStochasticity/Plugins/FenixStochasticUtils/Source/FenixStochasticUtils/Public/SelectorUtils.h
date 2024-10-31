// Copyright 2024, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "RandomSelector.h"

#include "SelectorUtils.generated.h"

/**
 * 
 */
UCLASS()
class FENIXSTOCHASTICUTILS_API USelectorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Create and return a RandomSelector."))
	static URandomSelector* CreateRandomSelector(const FRandomSelectorConfig& config);

	UFUNCTION(BlueprintPure, meta = (ToolTip = "Compute cumulative weights from weights, negative weights are regarded as zeros."))
	static TArray<double> GetCumWeights(const TArray<double>& weights);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Select index with given cumulative weights, negative means failure. Require non-negative and non-decreasing."))
	static int32 SelectWithCumWeights(const TArray<double>& cum_weights);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Select index with given weights, negative means failure."))
	static int32 SelectWithWeights(const TArray<double>& weights);
};
