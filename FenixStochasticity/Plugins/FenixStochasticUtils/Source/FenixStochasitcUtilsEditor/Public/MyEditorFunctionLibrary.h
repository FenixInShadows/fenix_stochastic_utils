// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyEditorFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FENIXSTOCHASTICUTILSEDITOR_API UMyEditorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static void MarkPackageDirty(UObject* Object);
};
