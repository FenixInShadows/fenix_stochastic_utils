// Copyright 2025, Tiannan Chen, All rights reserved.

#include "FenixStochasticUtilsEditor.h"
#include "TestCustomDetails.h"
#include "TestGenericDetailCustomization.h"
#include "TestCustomPropertyType.h"
#include "MyActor.h"
// #include "SMyBlueprint.h"

#define LOCTEXT_NAMESPACE "FFenixStochasticUtilsEditorModule"

void FFenixStochasticUtilsEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(AMyActor::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FTestCustomDetails::MakeInstance));
    PropertyModule.RegisterCustomPropertyTypeLayout(FMyStruct::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FTestCustomPropertyType::MakeInstance));

    FBlueprintEditorModule& BlueprintEditorModule = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
    BlueprintEditorModule.OnRegisterTabsForEditor().AddRaw(this, &FFenixStochasticUtilsEditorModule::HandleRegisterBlueprintEditorTab);
}

void FFenixStochasticUtilsEditorModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomClassLayout(AMyActor::StaticClass()->GetFName());
    PropertyModule.UnregisterCustomPropertyTypeLayout(FMyStruct::StaticStruct()->GetFName());
}

void FFenixStochasticUtilsEditorModule::HandleRegisterBlueprintEditorTab(FWorkflowAllowedTabSet& AllowedTabSet, FName ModeName, TSharedPtr<FBlueprintEditor> InEditor)
{
    /*if (InEditor->GetBlueprintObj()->GeneratedClass->IsChildOf(URandomSelector::StaticClass()))
    {
        InEditor->GetMyBlueprintWidget()->SetVisibility(EVisibility::Hidden);
    }*/

    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    TSharedPtr<IDetailsView> DetailsView = PropertyModule.FindDetailView("BlueprintInspector");
    if (DetailsView.IsValid())
    {
        DetailsView->SetGenericLayoutDetailsDelegate(FOnGetDetailCustomizationInstance::CreateStatic(&FTestGenericDetailCustomization::MakeInstance));
    }
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFenixStochasticUtilsEditorModule, FenixStochasticUtilsEditor)