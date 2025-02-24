// Copyright 2025, Tiannan Chen, All rights reserved.


#include "K2Node_RandomSelect.h"
#include "K2Node_CallFunction.h"
#include "CommonEditorUtils.h"
#include "KismetCompiler.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Engine/DataTable.h"
#include "SelectorUtils.h"
#include "CommonUtils.h"
#include "Kismet/BlueprintMapLibrary.h"
#include "Kismet/DataTableFunctionLibrary.h"

FText UK2Node_RandomSelect::GetTooltipText() const
{
	return FText();
}

void UK2Node_RandomSelect::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegister) const
{
	UClass* ActionKey = GetClass();
	if (ActionRegister.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner);
		ActionRegister.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

void UK2Node_RandomSelect::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

}

void UK2Node_RandomSelect::AllocateDefaultPins()
{
	// Add execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);

	Super::AllocateDefaultPins();
}

void UK2Node_RandomSelect::PinDefaultValueChanged(UEdGraphPin* ChangedPin)
{
	Super::PinDefaultValueChanged(ChangedPin);

}

void UK2Node_RandomSelect::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::PinConnectionListChanged(Pin);

}

void UK2Node_RandomSelect::PostReconstructNode()
{
	Super::PostReconstructNode();

}
