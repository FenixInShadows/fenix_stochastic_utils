// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "CommonDeveloperTypes.h"

#include "K2Node_RandomSelect.generated.h"


/**
 * Custom node for random selection.
 */
UCLASS()
class FENIXSTOCHASTICUTILSDEVELOPER_API UK2Node_RandomSelect : public UK2Node
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

	void CreateInOutPins();

	void OnDataTypePinUpdated(UEdGraphPin* ChangedPin);

	void OnFormatPinUpdated(UEdGraphPin* ChangedPin);

	void OnUseCookedInputPinUpdated(UEdGraphPin* ChangedPin);

	void OnUseStreamPinUpdated(UEdGraphPin* ChangedPin);

	void OnInputPinUpdated(UEdGraphPin* ChangedPin);

	void OnDataTableWeightOrProbNamePinUpdated(UEdGraphPin* ChangedPin);

	void OnDataTableIsProbNamePinUpdated(UEdGraphPin* ChangedPin);

	FText GetCurrentTooltip() const;

	UEdGraphPin* GetDataTypePin();

	UEdGraphPin* GetFormatPin();

	UEdGraphPin* GetUseCookedInputPin();

	UEdGraphPin* GetUseStreamPin();

	UEdGraphPin* GetInputPin();

	UEdGraphPin* GetInputDataTableWeightOrProbNamePin();

	UEdGraphPin* GetInputDataTableIsProbNamePin();

	UEdGraphPin* GetStreamPin();

	UEdGraphPin* GetOutputPin();

	UEdGraphPin* GetOutputKeyPin();

	FNodeTextCache CachedToolTip;

	UPROPERTY()  // Need to store this in asset, plus need to use this in ExpandNode for the temporary node copy.
	EFenixSelectorInputDataType CurrentDataType = EFenixSelectorInputDataType::Weight;

	UPROPERTY()  // Need to store this in asset, plus need to use this in ExpandNode for the temporary node copy.
	EFenixSelectorInputFormat CurrentFormat = EFenixSelectorInputFormat::Array;

	UPROPERTY()  // Need to store this in asset, plus need to use this in ExpandNode for the temporary node copy.
	bool UseCookedInput = false;
	
	UPROPERTY()  // Need to store this in asset, plus need to use this in ExpandNode for the temporary node copy.
	bool UseStream = false;

	UPROPERTY()  // Store this in asset for maintaining history/preference.
	TObjectPtr<UObject> DataTable;

	UPROPERTY()  // Store this in asset for maintaining history/preference.
	FString WeightPropertyName = "Weight";

	UPROPERTY()  // Store this in asset for maintaining history/preference.
	FString ProbPropertyName = "Prob";

	UPROPERTY()  // Store this in asset for maintaining history/preference.
	FString WeightOrProbPropertyName = "WeightOrProb";

	UPROPERTY()  // Store this in asset for maintaining history/preference.
	FString IsProbPropertyName = "IsProb";
};
