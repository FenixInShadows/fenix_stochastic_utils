// Copyright 2025, Tiannan Chen, All rights reserved.

#include "FenixStochasticUtilsEditor.h"
#include "TestCustomDetails.h"
#include "TestCustomDetailsForSceneComponent.h"
#include "TestCustomDetailsForEditorUtilityWidget.h"
#include "TestGenericDetailCustomization.h"
#include "TestCustomPropertyType.h"
#include "MyActor.h"
#include "ToolMenus.h"
#include "MyEditorFunctionLibrary.h"
#include "Misc/ScopedSlowTask.h"

#define LOCTEXT_NAMESPACE "FFenixStochasticUtilsEditorModule"

void FFenixStochasticUtilsEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(AMyActor::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FTestCustomDetails::MakeInstance));
    PropertyModule.RegisterCustomClassLayout(USceneComponent::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FTestCustomDetailsForSceneComponent::MakeInstance));
    
    PropertyModule.RegisterCustomClassLayout("TestEditorUtilityWidget_C", FOnGetDetailCustomizationInstance::CreateStatic(&FTestCustomDetailsForEditorUtilityWidget::MakeInstance));
    
    PropertyModule.RegisterCustomPropertyTypeLayout(FMyStruct::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FTestCustomPropertyType::MakeInstance));

    FBlueprintEditorModule& BlueprintEditorModule = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
    OnRegisterTabHandle = BlueprintEditorModule.OnRegisterTabsForEditor().AddRaw(this, &FFenixStochasticUtilsEditorModule::HandleRegisterBlueprintEditorTab);

    // Toolbars & Menus
    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(
        this, &FFenixStochasticUtilsEditorModule::RegisterMenuExtensions));
}

void FFenixStochasticUtilsEditorModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomClassLayout(AMyActor::StaticClass()->GetFName());
    PropertyModule.UnregisterCustomClassLayout(USceneComponent::StaticClass()->GetFName());
    PropertyModule.UnregisterCustomPropertyTypeLayout(FMyStruct::StaticStruct()->GetFName());

    FBlueprintEditorModule& BlueprintEditorModule = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
    BlueprintEditorModule.OnRegisterTabsForEditor().Remove(OnRegisterTabHandle);

    // Toolbars & Menus
    UToolMenus::UnRegisterStartupCallback(this);  // here "this" corresponds to the one used in CreateRaw in RegisterStartupCallback
    UToolMenus::UnregisterOwner(this);  // here "this" corresponds to the one used to create OwnerScoped
}

void FFenixStochasticUtilsEditorModule::HandleRegisterBlueprintEditorTab(FWorkflowAllowedTabSet& AllowedTabSet, FName ModeName, TSharedPtr<FBlueprintEditor> InEditor)
{
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

    TSharedPtr<IDetailsView> DetailsView = PropertyModule.FindDetailView("BlueprintInspector");
    if (DetailsView.IsValid())
    {
        DetailsView->SetGenericLayoutDetailsDelegate(FOnGetDetailCustomizationInstance::CreateStatic(&FTestGenericDetailCustomization::MakeInstance));
    }
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
        "MyCustomButton",
        FExecuteAction::CreateLambda([]()
            {
                EAppReturnType::Type Answer = FMessageDialog::Open(EAppMsgType::YesNo, FText::FromString("MyCustomButtonClicked"));
                FText SlateNotifyMsg = FText::FromString("Nothing Selected");
                switch (Answer)
                {
                case EAppReturnType::No:
                    SlateNotifyMsg = FText::FromString("No Selected");
                    break;
                case EAppReturnType::Yes:
                    SlateNotifyMsg = FText::FromString("Yes Selected");
                    break;
                }
                UMyEditorFunctionLibrary::NotifySuccess(
                    SlateNotifyMsg,
                    "https://space.bilibili.com/298437",
                    FText::FromString("Open this for something cooooool!")
                );
            }),
        INVTEXT("My custom button"),
        INVTEXT("Tooltip for my custom button"),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Comment")
    ));

    // Menu extension example
    UToolMenu* SelectionMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Select");
    FToolMenuSection& SelectionSection = SelectionMenu->AddSection(
        "MyCustomSelection",
        INVTEXT("My Custom Section"),
        FToolMenuInsert("SelectBSP", EToolMenuInsertType::After)
    );
    SelectionSection.AddEntry(FToolMenuEntry::InitMenuEntry(
        "MyCustomEntry",
        INVTEXT("My custom entry"),
        INVTEXT("Tooltip for my custom entry"),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Comment"),
        FExecuteAction::CreateLambda([]()
            {
                const int32 NumTestInterations = 10000000;
                int32 NumZeros = 0;
                FScopedSlowTask MyCustomSlowTask(float(NumTestInterations), FText::FromString("MyCustomEntry Task Running..."));
                MyCustomSlowTask.MakeDialog();
                for (int32 Index = 0; Index < NumTestInterations; Index++)
                {
                    MyCustomSlowTask.EnterProgressFrame(1.0f,
                        FText::Format(FText::FromString("Progress: {0}/{1}"), Index, NumTestInterations));
                    int32 RandomBit = FMath::RandRange(0, 1);
                    if (RandomBit == 0)
                    {
                        NumZeros++;
                    }   
                }
                MyCustomSlowTask.EnterProgressFrame(0.0f,
                    FText::Format(FText::FromString("Progress: {0}/{1}"), NumTestInterations, NumTestInterations));

                UMyEditorFunctionLibrary::NotifySuccess(
                    FText::Format(
                        FText::FromString("MyCustomEntry Task Completed.\n Number of Zeros: {0}/{1}"),
                        NumZeros,
                        NumTestInterations
                    ),
                    "https://space.bilibili.com/298437",
                    FText::FromString("Open this for something cooooool!")
                );
            })
    ));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFenixStochasticUtilsEditorModule, FenixStochasticUtilsEditor)