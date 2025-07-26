// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OnestopCacheDataAsset.generated.h"

/**
 * We generally don't save this asset after modifying in code beside specific cases,
 * (e.g. after the code created it needs to be saved, otherwise we don't; we allow manual modify and save in editor).
 */
UCLASS()
class FENIXSTOCHASTICUTILSEDITOR_API UOnestopCacheDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString CachedSearchKeyText = TEXT("");

	UPROPERTY(EditAnywhere)
	FString CachedSelectedCategoryLabel = TEXT("常用");
};
