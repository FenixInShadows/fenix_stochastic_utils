// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "MyEditorUtilityWidget.generated.h"

/**
 * 
 */
UCLASS()
class FENIXSTOCHASTICUTILSEDITOR_API UMyEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReplaceMaterial")
	double MyCppValue = 1.0;
};
