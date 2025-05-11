// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RandomSelector.generated.h"

//USTRUCT(BlueprintType)
//struct FENIXSTOCHASTICUTILS_API FRandomSelectorConfig
//{
//	GENERATED_BODY()
//
//};

/**
 * Blah Blah
 */
UCLASS(BlueprintType, Blueprintable)
class FENIXSTOCHASTICUTILS_API URandomSelector : public UObject
{
	GENERATED_BODY()
	
public:
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)*/
	UPROPERTY(EditDefaultsOnly)
	double testFloat = 2.0;
};
