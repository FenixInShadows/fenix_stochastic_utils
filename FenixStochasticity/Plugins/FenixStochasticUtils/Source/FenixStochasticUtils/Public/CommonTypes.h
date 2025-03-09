// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"

#define PIN_NAME_DATA_TYPE (TEXT("DataType"))
#define PIN_NAME_FORMAT (TEXT("Format"))
#define PIN_NAME_WEIGHTS (TEXT("Weights"))
#define PIN_NAME_PROBS (TEXT("Probs"))
#define PIN_NAME_WEIGHT_OR_PROBS (TEXT("WeightOrProbs"))
#define PIN_NAME_WEIGHT_MAP (TEXT("WeightMap"))
#define PIN_NAME_PROB_MAP (TEXT("ProbMap"))
#define PIN_NAME_WEIGHT_OR_PROB_MAP (TEXT("WeightOrProbMap"))
#define PIN_NAME_DATA_TABLE (TEXT("DataTable"))
#define PIN_NAME_WEIGHT_PROPERTY_NAME (TEXT("WeightPropertyName"))
#define PIN_NAME_PROB_PROPERTY_NAME (TEXT("ProbPropertyName"))
#define PIN_NAME_WEIGHT_OR_PROB_PROPERTY_NAME (TEXT("WeightOrProbPropertyName"))
#define PIN_NAME_IS_PROB_PROPERTY_NAME (TEXT("IsProbPropertyName"))
#define PIN_NAME_CUM_WEIGHTS (TEXT("CumWeights"))
#define PIN_NAME_CUM_PROBS (TEXT("CumProbs"))
#define PIN_NAME_COOKED_DISTRIBUTION (TEXT("CookedDistribution"))
#define PIN_NAME_KEYS (TEXT("Keys"))
#define PIN_NAME_ROW_NAMES (TEXT("RowNames"))
#define PIN_NAME_USE_COOKED_INPUT (TEXT("UseCookedInput"))
#define PIN_NAME_USE_STREAM (TEXT("UseStream"))
#define PIN_NAME_RANDOM_STREAM (TEXT("RandomStream"))
#define PIN_NAME_SELECTED_INEX (TEXT("SelectedIndex"))
#define PIN_NAME_SELECTED_KEY (TEXT("SelectedKey"))
#define PIN_NAME_SELECTED_ROW_NAME (TEXT("SelectedRowName"))

UENUM(BlueprintType)
enum class EFenixSelectorInputDataType : uint8
{
	Weight = 0,
	Prob = 1,
	WeightOrProb = 2
};

UENUM(BlueprintType)
enum class EFenixSelectorInputFormat : uint8
{
	Array = 0,
	Map = 1,
	DataTable = 2
};
