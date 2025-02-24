// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_RandomSelect.generated.h"



// InputPins:
// DataType 
// Format (only exist if not using cooked input)
// bool UseCookedInput
// bool UseStream;
// Input
// Stream

// OutputPins:
// Output



/**
 * Custom node for random selection.
 */
UCLASS()
class FENIXSTOCHASTICUTILS_API UK2Node_RandomSelect : public UK2Node
{
	GENERATED_BODY()
	
	virtual FText GetTooltipText() const override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString("Random Select"); }

	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegister) const override;

	virtual FText GetMenuCategory() const override { return FText::FromString("Fenix|SelectorUtils|Selection"); }

	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

	virtual void AllocateDefaultPins() override;

	virtual void PinDefaultValueChanged(UEdGraphPin* ChangedPin) override;

	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;

	virtual void PostReconstructNode() override;
};
