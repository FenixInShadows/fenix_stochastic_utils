// Copyright 2025, Tiannan Chen, All rights reserved.


#include "OnestopEditorUtilityWidget.h"
#include "Blueprint/WidgetTree.h"
#include "EditorUtilitySubsystem.h"
#include "LevelEditorSubsystem.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "Components/HorizontalBoxSlot.h"
#include "MyEditorFunctionLibrary.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "Factories/DataAssetFactory.h"

const double UOnestopEditorUtilityWidget::WeightDecayFactor = 0.9375;
const FString UOnestopEditorUtilityWidget::LocalFolderPath = "/FenixStochasticUtils/Demos/Demo_OnestopEditorTool/Local";
const FString UOnestopEditorUtilityWidget::PrefAssetName = "OnestopToolPref";
const FString UOnestopEditorUtilityWidget::CacheAssetName = "OnestopToolCache";
UOnestopPrefDataAsset* UOnestopEditorUtilityWidget::PrefAsset = nullptr;
UOnestopCacheDataAsset* UOnestopEditorUtilityWidget::CacheAsset = nullptr;

const TArray<OnestopActionEntryConfig> UOnestopEditorUtilityWidget::CustomActionEntryConfigs =
{
	{TEXT("工具"), TEXT("工具面板A"), &UOnestopEditorUtilityWidget::OpenToolPanelA},
	{TEXT("工具"), TEXT("工具1"), &UOnestopEditorUtilityWidget::OpenTool1},
	{TEXT("工具"), TEXT("工具2"), &UOnestopEditorUtilityWidget::OpenTool2},
	{TEXT("工具"), TEXT("工具3"), &UOnestopEditorUtilityWidget::OpenTool3},
	{TEXT("工具"), TEXT("工具4"), &UOnestopEditorUtilityWidget::OpenTool4},
	{TEXT("工具"), TEXT("工具5"), &UOnestopEditorUtilityWidget::OpenTool5},
	{TEXT("工具"), TEXT("工具6"), &UOnestopEditorUtilityWidget::OpenTool6},
	{TEXT("工具"), TEXT("工具7"), &UOnestopEditorUtilityWidget::OpenTool7},
	{TEXT("工具"), TEXT("工具8"), &UOnestopEditorUtilityWidget::OpenTool8},
	{TEXT("工具"), TEXT("工具9"), &UOnestopEditorUtilityWidget::OpenTool9},
	{TEXT("工具"), TEXT("工具10"), &UOnestopEditorUtilityWidget::OpenTool10},
	{TEXT("工具"), TEXT("工具11"), &UOnestopEditorUtilityWidget::OpenTool11},
	{TEXT("工具"), TEXT("工具12"), &UOnestopEditorUtilityWidget::OpenTool12},
	{TEXT("工具"), TEXT("工具13"), &UOnestopEditorUtilityWidget::OpenTool13},
	{TEXT("工具"), TEXT("工具14"), &UOnestopEditorUtilityWidget::OpenTool14},
	{TEXT("工具"), TEXT("工具15"), &UOnestopEditorUtilityWidget::OpenTool15},
	{TEXT("工具"), TEXT("工具16"), &UOnestopEditorUtilityWidget::OpenTool16},
	{TEXT("工具"), TEXT("工具17"), &UOnestopEditorUtilityWidget::OpenTool17},

	{TEXT("关卡"), TEXT("Lv1"), &UOnestopEditorUtilityWidget::OpenLevel1},
	{TEXT("关卡"), TEXT("Lv2"), &UOnestopEditorUtilityWidget::OpenLevel2},

	{TEXT("Actor"), TEXT("MyActor"), &UOnestopEditorUtilityWidget::OpenMyActor},

	{TEXT("检查"), TEXT("命名检查"), &UOnestopEditorUtilityWidget::CheckAssetNames},
};

// Note: don't add custom entries to here; add stuff only if really needed
const TArray<OnestopSpecificCategoryActionEntryConfig> UOnestopEditorUtilityWidget::ThisPanelActionEntryConfigs =
{
	{TEXT("清空书签"), &UOnestopEditorUtilityWidget::ClearBookmarks},
	{TEXT("清空常用"), &UOnestopEditorUtilityWidget::ClearCommonlyUsedActions},
};

void UOnestopEditorUtilityWidget::NotifyHelper(const FString& Msg)
{
	UMyEditorFunctionLibrary::NotifySuccess(FText::FromString(Msg), "", FText());
}

#pragma region Action Specific Functions for Custom Categories
void UOnestopEditorUtilityWidget::OpenToolPanelA()
{
	UEditorUtilityWidgetBlueprint* ToolPanelABlueprint = LoadObject<UEditorUtilityWidgetBlueprint>(
		nullptr, TEXT("/Script/Blutility.EditorUtilityWidgetBlueprint'/FenixStochasticUtils/Demos/Demo_OnestopEditorTool/ToolPanelA.ToolPanelA'"));
	if (ToolPanelABlueprint)
	{
		GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>()->SpawnAndRegisterTab(ToolPanelABlueprint);
	}
}

void UOnestopEditorUtilityWidget::OpenTool1()
{
	NotifyHelper("Open Tool1");
}

void UOnestopEditorUtilityWidget::OpenTool2()
{
	NotifyHelper("Open Tool2");
}

void UOnestopEditorUtilityWidget::OpenTool3()
{
	NotifyHelper("Open Tool3");
}

void UOnestopEditorUtilityWidget::OpenTool4()
{
	NotifyHelper("Open Tool4");
}

void UOnestopEditorUtilityWidget::OpenTool5()
{
	NotifyHelper("Open Tool5");
}

void UOnestopEditorUtilityWidget::OpenTool6()
{
	NotifyHelper("Open Tool6");
}

void UOnestopEditorUtilityWidget::OpenTool7()
{
	NotifyHelper("Open Tool7");
}

void UOnestopEditorUtilityWidget::OpenTool8()
{
	NotifyHelper("Open Tool8");
}

void UOnestopEditorUtilityWidget::OpenTool9()
{
	NotifyHelper("Open Tool9");
}

void UOnestopEditorUtilityWidget::OpenTool10()
{
	NotifyHelper("Open Tool10");
}

void UOnestopEditorUtilityWidget::OpenTool11()
{
	NotifyHelper("Open Tool11");
}

void UOnestopEditorUtilityWidget::OpenTool12()
{
	NotifyHelper("Open Tool12");
}

void UOnestopEditorUtilityWidget::OpenTool13()
{
	NotifyHelper("Open Tool13");
}

void UOnestopEditorUtilityWidget::OpenTool14()
{
	NotifyHelper("Open Tool14");
}

void UOnestopEditorUtilityWidget::OpenTool15()
{
	NotifyHelper("Open Tool15");
}

void UOnestopEditorUtilityWidget::OpenTool16()
{
	NotifyHelper("Open Tool16");
}

void UOnestopEditorUtilityWidget::OpenTool17()
{
	NotifyHelper("Open Tool17");
}

void UOnestopEditorUtilityWidget::OpenLevel1()
{
	GEditor->GetEditorSubsystem<ULevelEditorSubsystem>()->LoadLevel("/Script/Engine.World'/FenixStochasticUtils/Demos/Demo_OnestopEditorTool/Level1.Level1'");
}

void UOnestopEditorUtilityWidget::OpenLevel2()
{
	GEditor->GetEditorSubsystem<ULevelEditorSubsystem>()->LoadLevel("/Script/Engine.World'/FenixStochasticUtils/Demos/Demo_OnestopEditorTool/Level2.Level2'");
}

void UOnestopEditorUtilityWidget::OpenMyActor()
{
	// the path here is pure path version
	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(
		TEXT("/FenixStochasticUtils/Demos/Demo_OnestopEditorTool/Test_BP_MyActor.Test_BP_MyActor"));
}

void UOnestopEditorUtilityWidget::CheckAssetNames()
{
	NotifyHelper("Asset Name Checked.");
}
#pragma endregion

#pragma region Action Specific Functions for ThisPanel Category
void UOnestopEditorUtilityWidget::ClearBookmarks()
{
	EAppReturnType::Type Answer = FMessageDialog::Open(EAppMsgType::YesNo,
		FText::FromString(TEXT("确定要清空书签？")));
	if (Answer == EAppReturnType::Yes)
	{
		for (const FString& Label : PrefAsset->BookmarkedActions)
		{
			if (ActionNameEntryMap.Contains(Label))
			{
				ActionNameEntryMap[Label]->InitIsBookmarked(false);
			}
		}
		PrefAsset->BookmarkedActions.Empty();
		UMyEditorFunctionLibrary::SaveAsset(PrefAsset);
		NotifyHelper(TEXT("书签已清空"));
	}
}

void UOnestopEditorUtilityWidget::ClearCommonlyUsedActions()
{
	EAppReturnType::Type Answer = FMessageDialog::Open(EAppMsgType::YesNo,
		FText::FromString(TEXT("确定要清空常用？")));
	if (Answer == EAppReturnType::Yes)
	{
		PrefAsset->ActionWeights.Empty();
		UMyEditorFunctionLibrary::SaveAsset(PrefAsset);
		NotifyHelper(TEXT("常用已清空"));
	}
}
#pragma endregion

bool UOnestopEditorUtilityWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	InitLocalAsset();

	InitCategoryEntry(SearchCategoryEntry, TEXT("搜索"), true);

	CreateScrollBoxCategoryEntry(BookmarkCategoryEntry, TEXT("书签"), true);

	CreateScrollBoxCategoryEntry(CommonlyUsedCategoryEntry, TEXT("常用"), true);

	PrecreateEmptyActionEntries();

	TArray<FString> CustomCategoryLabels;
	ProcessCustomActionEntriesConfigs(CustomCategoryLabels);
	CreateCustomCategories(CustomCategoryLabels);

	ProcessThisPanelActionEntriesConfigs();
	CreateScrollBoxCategoryEntry(ThisPanelCategoryEntry, TEXT("此面板"), true);

	// Read the cache from asset to circumvent issues of automatic reinitializing after opening a level
	SetSearchKeyText(CacheAsset->CachedSearchKeyText);  // do this before setting category as it matters if it is the search cateogry
	if (CategoryNameEntryMap.Contains(CacheAsset->CachedSelectedCategoryLabel))
	{
		SelectCategory(CategoryNameEntryMap[CacheAsset->CachedSelectedCategoryLabel]);
	}
	else
	{
		SelectCategory(CommonlyUsedCategoryEntry);  // if the one in the asset is illegal then by default select CommonlyUsed
	}

	return true;
}

void UOnestopEditorUtilityWidget::OnSearchKeyTextUpdated()
{
	if (SelectedCategoryEntry == SearchCategoryEntry)
	{
		ActionScrollBox->ClearChildren();
		FillSearchCategoryActionEntries();
	}
	else
	{
		if (!GetSearchKeyText().TrimStartAndEnd().IsEmpty())
		{
			SelectCategory(SearchCategoryEntry);  // this will refresh the scrollbox
		}
	}
}

UEditorUtilityWidget* UOnestopEditorUtilityWidget::CreateSubWidget(TSubclassOf<UEditorUtilityWidget> WidgetClass)
{
	// Copied and adapted from parts of UEditorUtilityWidgetBlueprint::CreateUtilityWidget
	 UEditorUtilityWidget* CreatedUMGWidget = CreateWidget<UEditorUtilityWidget>(this, WidgetClass);
	if (CreatedUMGWidget)
	{
		// Editor Utility is flagged as transient to prevent from dirty the World it's created in when a property added to the Utility Widget is changed
		CreatedUMGWidget->SetFlags(RF_Transient);

		// Also mark nested utility widgets as transient to prevent them from dirtying the world (since they'll be created via CreateWidget and not CreateUtilityWidget)
		TArray<UWidget*> AllWidgets;
		CreatedUMGWidget->WidgetTree->GetAllWidgets(AllWidgets);

		for (UWidget* Widget : AllWidgets)
		{
			if (Widget->IsA(UEditorUtilityWidget::StaticClass()))
			{
				Widget->SetFlags(RF_Transient);
				if (UPanelSlot* PanelSlot = Widget->Slot)  // cannot use "Slot" as there is a member called "Slot" here as well
				{
					PanelSlot->SetFlags(RF_Transient);
				}
			}
		}
	}
    return CreatedUMGWidget;
}

void UOnestopEditorUtilityWidget::InitLocalAsset()
{
	UMyEditorFunctionLibrary::CreateDirectoryTree(LocalFolderPath);

	PrefAsset = Cast<UOnestopPrefDataAsset>(UMyEditorFunctionLibrary::LoadAsset(PrefAssetName, LocalFolderPath));
	if (!PrefAsset)
	{
		PrefAsset = Cast<UOnestopPrefDataAsset>(UMyEditorFunctionLibrary::CreateAsset(
			PrefAssetName,
			LocalFolderPath,
			UOnestopPrefDataAsset::StaticClass(),
			NewObject<UDataAssetFactory>()
		));
	}

	CacheAsset = Cast<UOnestopCacheDataAsset>(UMyEditorFunctionLibrary::LoadAsset(CacheAssetName, LocalFolderPath));
	if (!CacheAsset)
	{
		CacheAsset = Cast<UOnestopCacheDataAsset>(UMyEditorFunctionLibrary::CreateAsset(
			CacheAssetName,
			LocalFolderPath,
			UOnestopCacheDataAsset::StaticClass(),
			NewObject<UDataAssetFactory>()
		));
	}
}

void UOnestopEditorUtilityWidget::InitCategoryEntry(UOnestopCategoryEntry* Entry, const FString& Label, bool bIsSpecial)
{
	Entry->CategoryLabel = Label;
	Entry->SetDisplayText(FText::FromString(Label));
	Entry->SetIsSpecialCategory(bIsSpecial);
	Entry->OnSelected.BindUObject(this, &UOnestopEditorUtilityWidget::SelectCategory);
	CategoryNameEntryMap.Add(Label, Entry);
}

void UOnestopEditorUtilityWidget::CreateScrollBoxCategoryEntry(UOnestopCategoryEntry*& Entry, const FString& Label, bool bIsSpecial)
{
	UEditorUtilityWidget* CatgoryWidget = CreateSubWidget(CategoryEntryClass);
	CategoryScrollBox->AddChild(CatgoryWidget);
	Entry = Cast<UOnestopCategoryEntry>(CatgoryWidget);
	InitCategoryEntry(Entry, Label, bIsSpecial);
}

void UOnestopEditorUtilityWidget::PrecreateEmptyActionEntries()
{
	// Pre compute empty entries; NumActionsPerRow - 1 of those should be enough
	for (int32 EmptyIndex = 0; EmptyIndex < NumActionsPerRow - 1; EmptyIndex++)
	{
		EmptyActionEntries.Add(CreateSubWidget(EmptyActionEntryClass));
	}
}

void UOnestopEditorUtilityWidget::ProcessCustomActionEntriesConfigs(TArray<FString>& OutCustomCategoryLabels)
{
	for (const OnestopActionEntryConfig& Config : CustomActionEntryConfigs)
	{
		const FString& CategoryLabel = Config.CategoryName;
		const FString& ActionLabel = Config.ActionName;
		UOnestopActionEntry* ActionEntry = Cast<UOnestopActionEntry>(CreateSubWidget(ActionEntryClass));
		ActionEntry->ActionLabel = ActionLabel;
		ActionEntry->SetDisplayText(FText::FromString(ActionLabel));
		ActionEntry->OnSelected.BindUObject(this, Config.Func);
		ActionEntry->PostSelected.BindUObject(this, &UOnestopEditorUtilityWidget::PostCustomActionSelected);
		if (PrefAsset->BookmarkedActions.Contains(ActionLabel))
		{
			ActionEntry->InitIsBookmarked(true);
		}
		ActionEntry->PostIsBookmarkedUpdate.BindUObject(this, &UOnestopEditorUtilityWidget::PostIsBookmarkedUpdate);
		ActionNameEntryMap.Add(ActionLabel, ActionEntry);
		AllCustomActionEntries.Add(ActionEntry);
		if (CustomCategoryActionsMap.Contains(CategoryLabel))
		{
			CustomCategoryActionsMap[CategoryLabel].Add(ActionEntry);
		}
		else
		{
			CustomCategoryActionsMap.Add(CategoryLabel, {ActionEntry});
			OutCustomCategoryLabels.Add(CategoryLabel);
		}
	}
}

void UOnestopEditorUtilityWidget::CreateCustomCategories(const TArray<FString>& CustomCategoryLabels)
{
	for (const FString& Label : CustomCategoryLabels)
	{
		UEditorUtilityWidget* CategoryWidget = CreateSubWidget(CategoryEntryClass);
		CategoryScrollBox->AddChild(CategoryWidget);
		UOnestopCategoryEntry* CategoryEntry = Cast<UOnestopCategoryEntry>(CategoryWidget);
		InitCategoryEntry(CategoryEntry, Label);
	}
}

void UOnestopEditorUtilityWidget::ProcessThisPanelActionEntriesConfigs()
{
	for (const OnestopSpecificCategoryActionEntryConfig& Config : ThisPanelActionEntryConfigs)
	{
		const FString& ActionLabel = Config.ActionName;
		UOnestopActionEntry* ActionEntry = Cast<UOnestopActionEntry>(CreateSubWidget(ActionEntryClass));
		ActionEntry->ActionLabel = ActionLabel;
		ActionEntry->SetDisplayText(FText::FromString(ActionLabel));
		ActionEntry->OnSelected.BindUObject(this, Config.Func);
		ActionEntry->InitIsBookmarkable(false);
		ActionNameEntryMap.Add(ActionLabel, ActionEntry);
		ThisPanelActionEntries.Add(ActionEntry);
	}
}

void UOnestopEditorUtilityWidget::SelectCategory(UOnestopCategoryEntry* Entry)
{
	if (Entry == SelectedCategoryEntry)
	{
		return;
	}

	SelectedCategoryEntry = Entry;

	// Intensionally not saving CacheAsset after modifying so restarting the engine would be use the manually set default one
	CacheAsset->CachedSelectedCategoryLabel = Entry->CategoryLabel;

	if (Entry != SearchCategoryEntry)
	{
		ResetSearchState();
	}

	SelectCategoryEntry(Entry);
	RefreshActionEntriesForCategory(Entry);
}

void UOnestopEditorUtilityWidget::SelectCategoryEntry(UOnestopCategoryEntry* Entry)
{
	ClearSelectedCategoryEntry();
	Entry->MarkSelectionState(true);
}

void UOnestopEditorUtilityWidget::ClearSelectedCategoryEntry()
{
	SearchCategoryEntry->MarkSelectionState(false);

	TArray<UWidget*> ScrollBoxChildren = CategoryScrollBox->GetAllChildren();
	for (UWidget* ScrollBoxChild : ScrollBoxChildren)
	{
		if (UOnestopCategoryEntry* Entry = Cast<UOnestopCategoryEntry>(ScrollBoxChild))
		{
			Entry->MarkSelectionState(false);
		}
	}
}

void UOnestopEditorUtilityWidget::RefreshActionEntriesForCategory(const UOnestopCategoryEntry* CategoryEntry)
{
	ActionScrollBox->ClearChildren();

	if (CategoryEntry == SearchCategoryEntry)
	{
		FillSearchCategoryActionEntries();
	}
	else if (CategoryEntry == BookmarkCategoryEntry)
	{
		FillBookmarkCategoryActionEntries();
	}
	else if (CategoryEntry == CommonlyUsedCategoryEntry)
	{
		FillCommonlyUsedCategoryActionEntries();
	}
	else if (CategoryEntry == ThisPanelCategoryEntry)
	{
		FillThisPanelCategoryActionEntries();
	}
	else
	{
		FillCustomCategoryActionEntries(CategoryEntry);
	}
}

void UOnestopEditorUtilityWidget::FillSearchCategoryActionEntries()
{
	FString SearchKeyText = GetSearchKeyText();
	CacheAsset->CachedSearchKeyText = SearchKeyText;  // Intensionally not saving CacheAsset after modifying so restarting the engine would be use the manually set default one

	FString TrimedSearchKeyText = SearchKeyText.TrimStartAndEnd();
	if (TrimedSearchKeyText.IsEmpty())
	{
		return;
	}

	TArray<UOnestopActionEntry*> ActionEntries;
	for (UOnestopActionEntry* Entry : AllCustomActionEntries)
	{
		if (Entry->ActionLabel.Contains(TrimedSearchKeyText))
		{
			ActionEntries.Add(Entry);
		}
	}
	for (UOnestopActionEntry* Entry : ThisPanelActionEntries)
	{
		if (Entry->ActionLabel.Contains(TrimedSearchKeyText))
		{
			ActionEntries.Add(Entry);
		}
	}

	FillActionScrollBox(ActionEntries);
}

void UOnestopEditorUtilityWidget::FillBookmarkCategoryActionEntries()
{
	// Do reverse order, later added bookmarks are displayed first
	TArray<UOnestopActionEntry*> ActionEntries;
	for (int32 Index = PrefAsset->BookmarkedActions.Num() - 1; Index >= 0; Index--)
	{
		const FString& Label = PrefAsset->BookmarkedActions[Index];
		ActionEntries.Add(ActionNameEntryMap[Label]);
	}

	FillActionScrollBox(ActionEntries);
}

void UOnestopEditorUtilityWidget::FillCommonlyUsedCategoryActionEntries()
{
	TArray<FString> ActionLabels;
	PrefAsset->ActionWeights.GetKeys(ActionLabels);
	ActionLabels.Sort([](const FString& A, const FString& B) {
		return PrefAsset->ActionWeights[A] > PrefAsset->ActionWeights[B];
		});

	TArray<UOnestopActionEntry*> ActionEntries;
	for (const FString& Label : ActionLabels)
	{
		ActionEntries.Add(ActionNameEntryMap[Label]);
	}

	FillActionScrollBox(ActionEntries);
}

void UOnestopEditorUtilityWidget::FillThisPanelCategoryActionEntries()
{
	FillActionScrollBox(ThisPanelActionEntries);
}

void UOnestopEditorUtilityWidget::FillCustomCategoryActionEntries(const UOnestopCategoryEntry* CategoryEntry)
{
	const FString& CategoryLabel = CategoryEntry->CategoryLabel;
	if (CustomCategoryActionsMap.Contains(CategoryLabel))
	{
		FillActionScrollBox(CustomCategoryActionsMap[CategoryLabel]);
	}
}

void UOnestopEditorUtilityWidget::FillActionScrollBox(const TArray<UOnestopActionEntry*>& ActionEntries)
{
	const int32 NumEntries = ActionEntries.Num();
	const int32 NumFullRows = NumEntries / NumActionsPerRow;
	const int32 NumRemainderEntries = NumEntries % NumActionsPerRow;
	const int32 NumEmptyEntries = NumActionsPerRow - NumRemainderEntries;

	int32 RowStartIndex = 0;
	for (int32 RowIndex = 0; RowIndex < NumFullRows; RowIndex++, RowStartIndex += NumActionsPerRow)
	{
		UOnestopActionRow* Row = Cast<UOnestopActionRow>(CreateSubWidget(ActionRowClass));
		for (int32 ColumnIndex = 0; ColumnIndex < NumActionsPerRow; ColumnIndex++)
		{
			UOnestopActionEntry* Entry = ActionEntries[RowStartIndex + ColumnIndex];
			Row->RowBox->AddChild(Entry);
			Cast<UHorizontalBoxSlot>(Entry->Slot)->Size.SizeRule = ESlateSizeRule::Fill;
		}
		ActionScrollBox->AddChild(Row);
	}

	if (NumRemainderEntries > 0)
	{
		UOnestopActionRow* Row = Cast<UOnestopActionRow>(CreateSubWidget(ActionRowClass));
		for (int32 ColumnIndex = 0; ColumnIndex < NumRemainderEntries; ColumnIndex++)
		{
			UOnestopActionEntry* Entry = ActionEntries[RowStartIndex + ColumnIndex];
			Row->RowBox->AddChild(Entry);
			Cast<UHorizontalBoxSlot>(Entry->Slot)->Size.SizeRule = ESlateSizeRule::Fill;
		}
		for (int32 EmptyIndex = 0; EmptyIndex < NumEmptyEntries; EmptyIndex++)
		{
			UEditorUtilityWidget* EmptyEntry = EmptyActionEntries[EmptyIndex];
			Row->RowBox->AddChild(EmptyEntry);
			Cast<UHorizontalBoxSlot>(EmptyEntry->Slot)->Size.SizeRule = ESlateSizeRule::Fill;
		}
		ActionScrollBox->AddChild(Row);
	}
}

void UOnestopEditorUtilityWidget::PostCustomActionSelected(UOnestopActionEntry* Entry)
{
	for (auto It = PrefAsset->ActionWeights.CreateIterator(); It; ++It)
	{
		It.Value() *= WeightDecayFactor;
	}

	const FString& ActionLabel = Entry->ActionLabel;
	if (PrefAsset->ActionWeights.Contains(ActionLabel))
	{
		PrefAsset->ActionWeights[ActionLabel] += 1.0;
	}
	else
	{
		PrefAsset->ActionWeights.Add(ActionLabel, 1.0);
	}

	UMyEditorFunctionLibrary::SaveAsset(PrefAsset);
}

void UOnestopEditorUtilityWidget::PostIsBookmarkedUpdate(UOnestopActionEntry* Entry, const bool bBookmarked)
{
	const FString& ActionLabel = Entry->ActionLabel;

	const int IndexInArray = PrefAsset->BookmarkedActions.Find(ActionLabel);
	
	// If removing then this is needed;
	// If adding, though this shouldn't happen, but just in case we also first remove then add to refresh position 
	if (IndexInArray != INDEX_NONE)
	{
		PrefAsset->BookmarkedActions.RemoveAt(IndexInArray);
	}

	if (bBookmarked)
	{
		PrefAsset->BookmarkedActions.Add(ActionLabel);
	}

	UMyEditorFunctionLibrary::SaveAsset(PrefAsset);
}
