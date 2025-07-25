// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OnestopPrefDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FENIXSTOCHASTICUTILSEDITOR_API UOnestopPrefDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TMap<FString, double> ActionWeights;

	UPROPERTY(EditAnywhere)
	TArray<FString> BookmarkedActions;
};
