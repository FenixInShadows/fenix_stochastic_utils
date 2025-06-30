// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyPrimaryDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FENIXSTOCHASTICUTILS_API UMyPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name = "DefaultName";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double Value = 1.0;
};
