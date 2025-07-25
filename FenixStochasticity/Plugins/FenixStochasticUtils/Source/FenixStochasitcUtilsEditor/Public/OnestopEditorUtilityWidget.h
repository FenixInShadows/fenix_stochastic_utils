// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Components/ScrollBox.h"
#include "OnestopCategoryEntry.h"
#include "OnestopActionEntry.h"
#include "OnestopActionRow.h"
#include "OnestopPrefDataAsset.h"

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
	void PlaceholderActionHeler(FString Msg);
	
	void OpenToolPanelA();
	void OpenTool1();
	void OpenTool2();
	void OpenTool3();
	void OpenTool4();
	void OpenTool5();
	void OpenTool6();
	void OpenTool7();
	void OpenTool8();
	void OpenTool9();
	void OpenTool10();
	void OpenTool11();
	void OpenTool12();
	void OpenTool13();
	void OpenTool14();
	void OpenTool15();
	void OpenTool16();
	void OpenTool17();

	void OpenLevel1();
	void OpenLevel2();

	void OpenMyActor();

	void CheckAssetNames();
	#pragma endregion

public:	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOnestopCategoryEntry> CategoryEntryClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOnestopActionEntry> ActionEntryClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEditorUtilityWidget> ActionRowClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEditorUtilityWidget> EmptyActionEntryClass;

	UPROPERTY(EditAnywhere)
	int32 NumActionsPerRow = 4;

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

	UPROPERTY(Transient)
	TArray<UEditorUtilityWidget*> EmptyActionEntries;  // For filling incomplete rows.

public:
	virtual bool Initialize() override;

private:
	UEditorUtilityWidget* CreateSubWidget(TSubclassOf<UEditorUtilityWidget> WidgetClass);

	void ProcessCustomActionEntriesConfigs(TArray<FString>& OutCustomCategoryLabels);
	void CreateCustomCategories(const TArray<FString>& CustomCategoryLabels);

	void SelectCategory(UOnestopCategoryEntry* Entry);
	void SelectCategoryEntry(UOnestopCategoryEntry* Entry);
	void ClearSelectedCategoryEntry();
	void RefreshActionEntriesForCategory(const UOnestopCategoryEntry* Entry);
	void FillActionScrollBox(const TArray<UOnestopActionEntry*>& ActionEntries);

	static const FString PrefFolderPath;
	static const FString PrefAssetName;
	static UOnestopPrefDataAsset* PrefAsset;

	static const TArray<OnestopActionEntryConfig> CustomActionEntryConfigs;
};
