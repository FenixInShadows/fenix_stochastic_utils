// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Components/ScrollBox.h"
#include "OnestopCategoryEntry.h"
#include "OnestopActionEntry.h"
#include "OnestopActionRow.h"
#include "OnestopPrefDataAsset.h"
#include "OnestopCacheDataAsset.h"

#include "OnestopEditorUtilityWidget.generated.h"

struct FENIXSTOCHASTICUTILSEDITOR_API OnestopActionEntryConfig
{
	FString CategoryName;
	FString ActionName;
	void (UOnestopEditorUtilityWidget::* Func)();
};

struct FENIXSTOCHASTICUTILSEDITOR_API OnestopSpecificCategoryActionEntryConfig
{
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
	void NotifyHelper(const FString& Msg);

	#pragma region Action Specific Functions for Custom Categories	
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

	#pragma region Action Specific Functions for ThisPanel Category
	void ClearBookmarks();
	void ClearCommonlyUsedActions();
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

	TMap<FString, UOnestopCategoryEntry*> CategoryNameEntryMap;
	TMap<FString, UOnestopActionEntry*> ActionNameEntryMap;

	TMap<FString, TArray<UOnestopActionEntry*>> CustomCategoryActionsMap;	

	UPROPERTY(Transient)  // use UPROPERTY to prevent entry being GC'ed when switching panels etc. similar below
	TArray<UEditorUtilityWidget*> EmptyActionEntries;  // For filling incomplete rows.

	UPROPERTY(Transient)
	TArray<UOnestopActionEntry*> AllCustomActionEntries;

	UPROPERTY(Transient)
	TArray<UOnestopActionEntry*> ThisPanelActionEntries;

public:
	virtual bool Initialize() override;

	UFUNCTION(BlueprintImplementableEvent)  // SearchTextBox UI only
	void ResetSearchState();

	UFUNCTION(BlueprintImplementableEvent)
	FString GetSearchKeyText();

	UFUNCTION(BlueprintImplementableEvent)
	void SetSearchKeyText(const FString& KeyText);

	UFUNCTION(BlueprintCallable)
	void OnSearchKeyTextUpdated();

private:
	UEditorUtilityWidget* CreateSubWidget(TSubclassOf<UEditorUtilityWidget> WidgetClass);

	void InitLocalAsset();
	void InitCategoryEntry(UOnestopCategoryEntry* Entry, const FString& Label, bool bIsSpecial = false);
	void CreateScrollBoxCategoryEntry(UOnestopCategoryEntry*& Entry, const FString& Label, bool bIsSpecial = false);
	void PrecreateEmptyActionEntries();
	void ProcessCustomActionEntriesConfigs(TArray<FString>& OutCustomCategoryLabels);
	void CreateCustomCategories(const TArray<FString>& CustomCategoryLabels);
	void ProcessThisPanelActionEntriesConfigs();

	void SelectCategory(UOnestopCategoryEntry* Entry);
	void SelectCategoryEntry(UOnestopCategoryEntry* Entry);
	void ClearSelectedCategoryEntry();
	void RefreshActionEntriesForCategory(const UOnestopCategoryEntry* CategoryEntry);
	void FillSearchCategoryActionEntries();
	void FillBookmarkCategoryActionEntries();
	void FillCommonlyUsedCategoryActionEntries();
	void FillThisPanelCategoryActionEntries();
	void FillCustomCategoryActionEntries(const UOnestopCategoryEntry* CategoryEntry);
	void FillActionScrollBox(const TArray<UOnestopActionEntry*>& ActionEntries);

	void PostCustomActionSelected(UOnestopActionEntry* Entry);
	void PostIsBookmarkedUpdate(UOnestopActionEntry* Entry, const bool bBookmarked);

	UOnestopCategoryEntry* SelectedCategoryEntry = nullptr;

	static const double WeightDecayFactor;
	static const FString LocalFolderPath;
	static const FString PrefAssetName;
	static const FString CacheAssetName;
	static UOnestopPrefDataAsset* PrefAsset;
	static UOnestopCacheDataAsset* CacheAsset;

	static const TArray<OnestopActionEntryConfig> CustomActionEntryConfigs;
	static const TArray<OnestopSpecificCategoryActionEntryConfig> ThisPanelActionEntryConfigs;
};
