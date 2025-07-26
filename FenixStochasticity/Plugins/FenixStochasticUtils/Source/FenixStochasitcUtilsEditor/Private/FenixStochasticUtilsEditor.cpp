// Copyright 2025, Tiannan Chen, All rights reserved.

#include "FenixStochasticUtilsEditor.h"
#include "ToolMenus.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"

#define LOCTEXT_NAMESPACE "FFenixStochasticUtilsEditorModule"

void FFenixStochasticUtilsEditorModule::StartupModule()
{
	// Toolbars & Menus
	RegisterMenuExtensions();
}

void FFenixStochasticUtilsEditorModule::ShutdownModule()
{
	// Toolbars & Menus
	UToolMenus::UnregisterOwner(this);  // here "this" corresponds to the one used to create OwnerScoped
}

void FFenixStochasticUtilsEditorModule::RegisterMenuExtensions()
{
    // Use "this" as the owner of custom items added here (it'll be active for all changes made when this FToolMenuOwnerScoped is the most recent one created and active).
    // This allows the unregistration in ShutdownModule to use "this" to unregister and only unregister with it as the owner (cooresponds to parameters used in UnregisterOwner).
    FToolMenuOwnerScoped OwnerScoped(this);

    // Toolbar extension example
    UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.ModesToolBar");
    FToolMenuSection& ToolbarSection = ToolbarMenu->FindOrAddSection("File");
    ToolbarSection.AddEntry(FToolMenuEntry::InitToolBarButton(
        "OpenOnestopToolButton",
        FExecuteAction::CreateLambda([]()
            {
                UEditorUtilityWidgetBlueprint* WidgetBlueprint = LoadObject<UEditorUtilityWidgetBlueprint>(
                    nullptr, TEXT("/Script/Blutility.EditorUtilityWidgetBlueprint'/FenixStochasticUtils/Demos/Demo_OnestopEditorTool/OnestopEditorTool.OnestopEditorTool'"));
                if (WidgetBlueprint)
                {
                    GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>()->SpawnAndRegisterTab(WidgetBlueprint);
                }
            }),
        INVTEXT("Open Onestop Tool Button"),
        INVTEXT("Open Onestop Tool"),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Tools")
    ));

    // Menu extension example
    UToolMenu* SelectionMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
    FToolMenuSection& SelectionSection = SelectionMenu->AddSection(
        "CustomTool",
        INVTEXT("Custom Tool"),
        FToolMenuInsert("Programming", EToolMenuInsertType::Before)
    );
    SelectionSection.AddEntry(FToolMenuEntry::InitMenuEntry(
        "OpenOnestopToolEntry",
        INVTEXT("Open Onestop Tool Entry"),
        INVTEXT("Open Onestop Tool"),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Tools"),
        FExecuteAction::CreateLambda([]()
            {
                UEditorUtilityWidgetBlueprint* WidgetBlueprint = LoadObject<UEditorUtilityWidgetBlueprint>(
                    nullptr, TEXT("/Script/Blutility.EditorUtilityWidgetBlueprint'/FenixStochasticUtils/Demos/Demo_OnestopEditorTool/OnestopEditorTool.OnestopEditorTool'"));
                if (WidgetBlueprint)
                {
                    GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>()->SpawnAndRegisterTab(WidgetBlueprint);
                }
            })
    ));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFenixStochasticUtilsEditorModule, FenixStochasticUtilsEditor)