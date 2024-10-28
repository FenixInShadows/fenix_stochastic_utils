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

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Create and return a RandomSelector"))
	static URandomSelector* CreateRandomSelector(const FRandomSelectorConfig& config);
};
