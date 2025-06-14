// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "BlueprintEditor.h"

class FFenixStochasticUtilsEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void HandleRegisterBlueprintEditorTab(FWorkflowAllowedTabSet& AllowedTabSet, FName ModeName, TSharedPtr<FBlueprintEditor> InEditor);

private:
	FDelegateHandle OnRegisterTabHandle;
};
