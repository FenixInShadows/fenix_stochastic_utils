// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"

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
