// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "OnestopActionEntry.generated.h"


DECLARE_DELEGATE(FOnestopActionSelectedDelegate);
DECLARE_DELEGATE_OneParam(FOnestopActionPostSelectedDelegate, UOnestopActionEntry*)
DECLARE_DELEGATE_TwoParams(FOnestopActionPostBookmarkDelegate, UOnestopActionEntry*, const bool);

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

	UFUNCTION(BlueprintImplementableEvent)  // only for UI
	void SetBookmarkState(const bool bBookmarked);

	UFUNCTION(BlueprintImplementableEvent)  // only for UI
	void SetBookmarkableState(const bool bBookmarkable);

	UFUNCTION(BlueprintCallable)
	void SwitchIsBookmarked();

	void UpdateIsBookmarked(const bool bBookmarked);

	void InitIsBookmarked(const bool bBookmarked);
	
	void InitIsBookmarkable(const bool bBookmarkable);

	FString ActionLabel;

	bool bIsBookmarked = false;
	
	FOnestopActionSelectedDelegate OnSelected;
	FOnestopActionPostSelectedDelegate PostSelected;

	FOnestopActionPostBookmarkDelegate PostIsBookmarkedUpdate;
};
