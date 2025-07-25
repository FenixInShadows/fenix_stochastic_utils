// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Components/HorizontalBox.h"

#include "OnestopActionRow.generated.h"

/**
 * 
 */
UCLASS()
class FENIXSTOCHASTICUTILSEDITOR_API UOnestopActionRow : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* RowBox;
};
