// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "OnestopCategoryEntry.generated.h"

DECLARE_DELEGATE_OneParam(FCategorySelectedDelegate, UOnestopCategoryEntry*);

/**
 * 
 */
UCLASS()
class FENIXSTOCHASTICUTILSEDITOR_API UOnestopCategoryEntry : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetDisplayText(const FText& Description);

	UFUNCTION(BlueprintImplementableEvent)
	void MarkSelectionState(bool bSelected);

	UFUNCTION(BlueprintCallable)
	void SelectCategory();

	FString CategoryLabel;

	FCategorySelectedDelegate OnSelected;
};
