// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "OnestopActionEntry.generated.h"


DECLARE_DELEGATE(FActionSelectedDelegate);
DECLARE_DELEGATE_OneParam(FActionBookmarkedDelegate, UOnestopActionEntry*);

/**
 * 
 */
UCLASS()
class FENIXSTOCHASTICUTILSEDITOR_API UOnestopActionEntry : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetDisplayText(const FText& Description);

	UFUNCTION(BlueprintCallable)
	void SelectAction();
	
	FActionSelectedDelegate OnSelected;

	FActionBookmarkedDelegate OnBookmarked;
};
