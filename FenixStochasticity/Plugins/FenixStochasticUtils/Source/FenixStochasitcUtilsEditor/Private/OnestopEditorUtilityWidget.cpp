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

const FString UOnestopEditorUtilityWidget::PrefFolderPath = "/FenixStochasticUtils/Demos/Demo_OnestopEditorTool/Temp";
const FString UOnestopEditorUtilityWidget::PrefAssetName = "OnestopToolPref";
UOnestopPrefDataAsset* UOnestopEditorUtilityWidget::PrefAsset = nullptr;

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

#pragma region Action Entry Functions
void UOnestopEditorUtilityWidget::PlaceholderActionHeler(FString Msg)
{
	UMyEditorFunctionLibrary::NotifySuccess(FText::FromString(Msg), "", FText());
}

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
	PlaceholderActionHeler("Open Tool1");
}

void UOnestopEditorUtilityWidget::OpenTool2()
{
	PlaceholderActionHeler("Open Tool2");
}

void UOnestopEditorUtilityWidget::OpenTool3()
{
	PlaceholderActionHeler("Open Tool3");
}

void UOnestopEditorUtilityWidget::OpenTool4()
{
	PlaceholderActionHeler("Open Tool4");
}

void UOnestopEditorUtilityWidget::OpenTool5()
{
	PlaceholderActionHeler("Open Tool5");
}

void UOnestopEditorUtilityWidget::OpenTool6()
{
	PlaceholderActionHeler("Open Tool6");
}

void UOnestopEditorUtilityWidget::OpenTool7()
{
	PlaceholderActionHeler("Open Tool7");
}

void UOnestopEditorUtilityWidget::OpenTool8()
{
	PlaceholderActionHeler("Open Tool8");
}

void UOnestopEditorUtilityWidget::OpenTool9()
{
	PlaceholderActionHeler("Open Tool9");
}

void UOnestopEditorUtilityWidget::OpenTool10()
{
	PlaceholderActionHeler("Open Tool10");
}

void UOnestopEditorUtilityWidget::OpenTool11()
{
	PlaceholderActionHeler("Open Tool11");
}

void UOnestopEditorUtilityWidget::OpenTool12()
{
	PlaceholderActionHeler("Open Tool12");
}

void UOnestopEditorUtilityWidget::OpenTool13()
{
	PlaceholderActionHeler("Open Tool13");
}

void UOnestopEditorUtilityWidget::OpenTool14()
{
	PlaceholderActionHeler("Open Tool14");
}

void UOnestopEditorUtilityWidget::OpenTool15()
{
	PlaceholderActionHeler("Open Tool15");
}

void UOnestopEditorUtilityWidget::OpenTool16()
{
	PlaceholderActionHeler("Open Tool16");
}

void UOnestopEditorUtilityWidget::OpenTool17()
{
	PlaceholderActionHeler("Open Tool17");
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
	PlaceholderActionHeler("Asset Name Checked.");
}
#pragma endregion

bool UOnestopEditorUtilityWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (UMyEditorFunctionLibrary::IsDirectoryExist(PrefFolderPath))
	{
		UMyEditorFunctionLibrary::CreateDirectory(PrefFolderPath);
	}

	if (UMyEditorFunctionLibrary::IsAssetExist(PrefAssetName, PrefFolderPath))
	{
		PrefAsset = Cast<UOnestopPrefDataAsset>(UMyEditorFunctionLibrary::LoadAsset(PrefAssetName, PrefFolderPath));
	}
	else
	{
		PrefAsset = Cast<UOnestopPrefDataAsset>(UMyEditorFunctionLibrary::CreateAsset(
			PrefAssetName,
			PrefFolderPath,
			UOnestopPrefDataAsset::StaticClass(),
			NewObject<UDataAssetFactory>()
		));
	}

	// Debug
	if (PrefAsset->ActionWeights.Contains("Test"))
	{
		PrefAsset->ActionWeights["Test"] += 1.0;
	}
	else
	{
		PrefAsset->ActionWeights.Add("Test", 1.0);
	}
	UMyEditorFunctionLibrary::SaveAsset(PrefAsset);

	SearchCategoryEntry->CategoryLabel = TEXT("搜索");
	SearchCategoryEntry->SetDisplayText(FText::FromString(TEXT("搜索")));
	SearchCategoryEntry->OnSelected.BindUObject(this, &UOnestopEditorUtilityWidget::SelectCategory);

	TArray<FString> CustomActionLabels;
	ProcessCustomActionEntriesConfigs(CustomActionLabels);


	CreateCustomCategories(CustomActionLabels);

	UEditorUtilityWidget* ThisPanelWidget = CreateSubWidget(CategoryEntryClass);
	CategoryScrollBox->AddChild(ThisPanelWidget);
	ThisPanelCategoryEntry = Cast<UOnestopCategoryEntry>(ThisPanelWidget);
	ThisPanelCategoryEntry->CategoryLabel = TEXT("此面板");
	ThisPanelCategoryEntry->SetDisplayText(FText::FromString(TEXT("此面板")));
	ThisPanelCategoryEntry->OnSelected.BindUObject(this, &UOnestopEditorUtilityWidget::SelectCategory);

	return true;
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

void UOnestopEditorUtilityWidget::ProcessCustomActionEntriesConfigs(TArray<FString>& OutCustomCategoryLabels)
{
	// Pre compute action entries for custom categories
	for (const OnestopActionEntryConfig& Config : CustomActionEntryConfigs)
	{
		const FString& CategoryLabel = Config.CategoryName;
		const FString& ActionLabel = Config.ActionName;
		UOnestopActionEntry* ActionEntry = Cast<UOnestopActionEntry>(CreateSubWidget(ActionEntryClass));
		ActionEntry->SetDisplayText(FText::FromString(ActionLabel));
		ActionEntry->OnSelected.BindUObject(this, Config.Func);
		ActionNameEntryMap.Add(ActionLabel, ActionEntry);
		AllCustomActionEntries.Add(ActionEntry);
		if (CategoryActionsMap.Contains(CategoryLabel))
		{
			CategoryActionsMap[CategoryLabel].Add(ActionEntry);
		}
		else
		{
			CategoryActionsMap.Add(CategoryLabel, {ActionEntry});
			OutCustomCategoryLabels.Add(CategoryLabel);
		}
	}

	// Pre compute empty entries; NumActionsPerRow - 1 of those should be enough
	for (int32 EmptyIndex = 0; EmptyIndex < NumActionsPerRow - 1; EmptyIndex++)
	{
		EmptyActionEntries.Add(CreateSubWidget(EmptyActionEntryClass));
	}
}

void UOnestopEditorUtilityWidget::CreateCustomCategories(const TArray<FString>& CustomCategoryLabels)
{
	for (const FString& Label : CustomCategoryLabels)
	{
		UEditorUtilityWidget* CategoryWidget = CreateSubWidget(CategoryEntryClass);
		CategoryScrollBox->AddChild(CategoryWidget);
		UOnestopCategoryEntry* CategoryEntry = Cast<UOnestopCategoryEntry>(CategoryWidget);
		CategoryEntry->CategoryLabel = Label;
		CategoryEntry->SetDisplayText(FText::FromString(Label));
		CategoryEntry->OnSelected.BindUObject(this, &UOnestopEditorUtilityWidget::SelectCategory);
	}
}

void UOnestopEditorUtilityWidget::SelectCategory(UOnestopCategoryEntry* Entry)
{
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

void UOnestopEditorUtilityWidget::RefreshActionEntriesForCategory(const UOnestopCategoryEntry* Entry)
{
	ActionScrollBox->ClearChildren();

	if (Entry == SearchCategoryEntry)
	{

	}
	else if (Entry == BookmarkCategoryEntry)
	{
	}
	else if (Entry == CommonlyUsedCategoryEntry)
	{

	}
	else if (Entry == ThisPanelCategoryEntry)
	{

	}
	else
	{
		const FString& CategoryLabel = Entry->CategoryLabel;
		if (CategoryActionsMap.Contains(CategoryLabel))
		{
			FillActionScrollBox(CategoryActionsMap[CategoryLabel]);
		}
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
