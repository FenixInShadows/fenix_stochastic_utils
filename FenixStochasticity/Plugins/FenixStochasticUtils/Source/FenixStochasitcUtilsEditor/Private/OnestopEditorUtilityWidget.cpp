// Copyright 2025, Tiannan Chen, All rights reserved.


#include "OnestopEditorUtilityWidget.h"
#include "Blueprint/WidgetTree.h"
#include "LevelEditorSubsystem.h"

const TArray<OnestopActionEntryConfig> UOnestopEditorUtilityWidget::CustomActionEntryConfigs =
{
	{TEXT("关卡"), TEXT("Level 1"), &UOnestopEditorUtilityWidget::OpenLevel1},
	{TEXT("关卡"), TEXT("Level 2"), &UOnestopEditorUtilityWidget::OpenLevel2},
};

#pragma region Action Entry Functions
void UOnestopEditorUtilityWidget::OpenLevel1()
{
	GEditor->GetEditorSubsystem<ULevelEditorSubsystem>()->LoadLevel("/Script/Engine.World'/FenixStochasticUtils/Demos/Demo_OnestopEditorTool/Level1.Level1'");
}

void UOnestopEditorUtilityWidget::OpenLevel2()
{
	GEditor->GetEditorSubsystem<ULevelEditorSubsystem>()->LoadLevel("/Script/Engine.World'/FenixStochasticUtils/Demos/Demo_OnestopEditorTool/Level2.Level2'");
}
#pragma endregion

bool UOnestopEditorUtilityWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	SearchCategoryEntry->CategoryLabel = TEXT("搜索");
	SearchCategoryEntry->SetDisplayText(FText::FromString(TEXT("搜索")));
	SearchCategoryEntry->OnSelected.BindUObject(this, &UOnestopEditorUtilityWidget::SelectCategory);

	if (CategoryEntryClass)
	{
		TArray<FString> CustomActionLabels;
		ProcessCustomActionEntriesConfigs(CustomActionLabels);

		CreateCustomCategories(CustomActionLabels);

		UEditorUtilityWidget* ThisPanelWidget = CreateChildWidget(CategoryEntryClass);
		CategoryScrollBox->AddChild(ThisPanelWidget);
		ThisPanelCategoryEntry = Cast<UOnestopCategoryEntry>(ThisPanelWidget);
		ThisPanelCategoryEntry->CategoryLabel = TEXT("此面板");
		ThisPanelCategoryEntry->SetDisplayText(FText::FromString(TEXT("此面板")));
		ThisPanelCategoryEntry->OnSelected.BindUObject(this, &UOnestopEditorUtilityWidget::SelectCategory);
	}

	return true;
}

UEditorUtilityWidget* UOnestopEditorUtilityWidget::CreateChildWidget(TSubclassOf<UEditorUtilityWidget> WidgetClass)
{
	// Copied and adapted from parts of UEditorUtilityWidgetBlueprint::CreateUtilityWidget
	UEditorUtilityWidget* CreatedUMGWidget = nullptr;
	if (UWorld* World = GEditor->GetEditorWorldContext().World())
	{
		CreatedUMGWidget = CreateWidget<UEditorUtilityWidget>(World, WidgetClass);
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
	}
    return CreatedUMGWidget;
}

void UOnestopEditorUtilityWidget::ProcessCustomActionEntriesConfigs(TArray<FString>& CustomCategoryLabels)
{
	for (const OnestopActionEntryConfig& Config : CustomActionEntryConfigs)
	{
		const FString& CategoryLabel = Config.CategoryName;
		UOnestopActionEntry* ActionEntry = Cast<UOnestopActionEntry>(CreateChildWidget(ActionEntryClass));
		ActionEntry->SetDisplayText(FText::FromString(Config.ActionName));
		ActionEntry->OnSelected.BindUObject(this, Config.Func);
		if (CategoryActionsMap.Contains(CategoryLabel))
		{
			CategoryActionsMap[CategoryLabel].Add(ActionEntry);
		}
		else
		{
			CategoryActionsMap.Add(CategoryLabel, {ActionEntry});
			CustomCategoryLabels.Add(CategoryLabel);
		}
	}
}

void UOnestopEditorUtilityWidget::CreateCustomCategories(const TArray<FString>& CustomCategoryLabels)
{
	for (const FString& Label : CustomCategoryLabels)
	{
		UEditorUtilityWidget* CategoryWidget = CreateChildWidget(CategoryEntryClass);
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
			for (UOnestopActionEntry* ActionEntry : CategoryActionsMap[CategoryLabel])
			{
				ActionScrollBox->AddChild(ActionEntry);
			}
		}
	}
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
