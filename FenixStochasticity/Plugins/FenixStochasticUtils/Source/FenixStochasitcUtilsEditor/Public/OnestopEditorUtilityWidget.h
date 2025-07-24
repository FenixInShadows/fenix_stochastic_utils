// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Components/ScrollBox.h"
#include "OnestopCategoryEntry.h"
#include "OnestopActionEntry.h"

#include "OnestopEditorUtilityWidget.generated.h"

struct FENIXSTOCHASTICUTILSEDITOR_API OnestopActionEntryConfig
{
	FString CategoryName;
	FString ActionName;
	void (UOnestopEditorUtilityWidget::* Func)();
};

/**
 * 
 */
UCLASS()
class FENIXSTOCHASTICUTILSEDITOR_API UOnestopEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

private:
	#pragma region Action Entry Functions
	void OpenLevel1();
	void OpenLevel2();
	#pragma endregion
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOnestopCategoryEntry> CategoryEntryClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOnestopActionEntry> ActionEntryClass;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* CategoryScrollBox;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ActionScrollBox;

	UPROPERTY(meta = (BindWidget))
	UOnestopCategoryEntry* SearchCategoryEntry;

private:
	UOnestopCategoryEntry* BookmarkCategoryEntry;
	
	UOnestopCategoryEntry* CommonlyUsedCategoryEntry;

	// Entries in here generally do not support Bookmark or CommonlyUsed feature, but support searching.
	UOnestopCategoryEntry* ThisPanelCategoryEntry;

	TMap<FString, TArray<UOnestopActionEntry*>> CategoryActionsMap;

	TMap<FString, UOnestopActionEntry*> ActionNameEntryMap;

	UPROPERTY(Transient)  // use UPROPERTY to prevent entry being GC'ed when switching panels etc.
	TArray<UOnestopActionEntry*> AllCustomActionEntries;

public:
	virtual bool Initialize() override;

private:
	UEditorUtilityWidget* CreateChildWidget(TSubclassOf<UEditorUtilityWidget> WidgetClass);
	void ProcessCustomActionEntriesConfigs(TArray<FString>& CustomCategoryLabels);
	void CreateCustomCategories(const TArray<FString>& CustomCategoryLabels);

	void SelectCategory(UOnestopCategoryEntry* Entry);
	void RefreshActionEntriesForCategory(const UOnestopCategoryEntry* Entry);
	void SelectCategoryEntry(UOnestopCategoryEntry* Entry);
	void ClearSelectedCategoryEntry();

	static const TArray<OnestopActionEntryConfig> CustomActionEntryConfigs;
};
