// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class FENIXSTOCHASTICUTILS_API UMyDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name = "DefaultName";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double Value = 1.0;
};
