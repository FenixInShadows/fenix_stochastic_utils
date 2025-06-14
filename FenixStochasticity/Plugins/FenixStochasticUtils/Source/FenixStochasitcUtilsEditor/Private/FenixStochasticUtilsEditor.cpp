// Copyright 2025, Tiannan Chen, All rights reserved.

#include "FenixStochasticUtilsEditor.h"
#include "TestCustomDetails.h"
#include "TestCustomDetailsForSceneComponent.h"
#include "TestGenericDetailCustomization.h"
#include "TestCustomPropertyType.h"
#include "MyActor.h"

#define LOCTEXT_NAMESPACE "FFenixStochasticUtilsEditorModule"

void FFenixStochasticUtilsEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(AMyActor::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FTestCustomDetails::MakeInstance));
    PropertyModule.RegisterCustomClassLayout(USceneComponent::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FTestCustomDetailsForSceneComponent::MakeInstance));
    PropertyModule.RegisterCustomPropertyTypeLayout(FMyStruct::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FTestCustomPropertyType::MakeInstance));

    FBlueprintEditorModule& BlueprintEditorModule = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
    OnRegisterTabHandle = BlueprintEditorModule.OnRegisterTabsForEditor().AddRaw(this, &FFenixStochasticUtilsEditorModule::HandleRegisterBlueprintEditorTab);
}

void FFenixStochasticUtilsEditorModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomClassLayout(AMyActor::StaticClass()->GetFName());
    PropertyModule.UnregisterCustomClassLayout(USceneComponent::StaticClass()->GetFName());
    PropertyModule.UnregisterCustomPropertyTypeLayout(FMyStruct::StaticStruct()->GetFName());

    FBlueprintEditorModule& BlueprintEditorModule = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
    BlueprintEditorModule.OnRegisterTabsForEditor().Remove(OnRegisterTabHandle);
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

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFenixStochasticUtilsEditorModule, FenixStochasticUtilsEditor)