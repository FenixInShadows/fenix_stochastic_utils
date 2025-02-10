// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_CookSelectorInput.generated.h"


UENUM(BlueprintType)
enum class EFenixCookSelectorInputDataType : uint8
{
	Weight = 0,
	Prob = 1,
	WeightOrProb = 2
};

UENUM(BlueprintType)
enum class EFenixCookSelectorInputFormat : uint8
{
	Array = 0,
	Map = 1,
	DataTable = 2
};

/**
 * 
 */
UCLASS()
class FENIXSTOCHASTICUTILS_API UK2Node_CookSelectorInput : public UK2Node
{
	GENERATED_BODY()

	virtual FText GetTooltipText() const override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString("Cook Selector Input"); }

	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegister) const override;

	virtual FText GetMenuCategory() const override { return FText::FromString("Fenix|SelectorUtils|SelectionPreprocessing"); }

	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

	virtual void AllocateDefaultPins() override;
	
	virtual void PinDefaultValueChanged(UEdGraphPin* ChangedPin) override;

	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;

	virtual void PostReconstructNode() override;

	void CreateInOutPins();

	void OnDataTypePinUpdated(UEdGraphPin* ChangedPin);

	void OnFormatPinUpdated(UEdGraphPin* ChangedPin);

	void OnInputPinUpdated(UEdGraphPin* ChangedPin);

	void OnDataTableWeightOrProbNamePinUpdated(UEdGraphPin* ChangedPin);

	void OnDataTableIsProbNamePinUpdated(UEdGraphPin* ChangedPin);

	void OnPinConnectionUpdateInMapFormat(UEdGraphPin* Pin, UEdGraphPin* SyncedPin);

	bool PostPinConnectionReconstructionInMapFormat(UEdGraphPin* Pin, UEdGraphPin* SyncedPin);

	void ChangeToWildCardPinType(FEdGraphPinType& PinType);

	void CopyPinTypeCategoryInfo(FEdGraphPinType& PinType, const FEdGraphPinType& SrcPinType);

	void CopyPinTypeAndValueTypeInfo(FEdGraphPinType& PinType, const FEdGraphPinType& SrcPinType);

	void CopyPinValueTypeToPinTypeInfo(FEdGraphPinType& PinType, const FEdGraphTerminalType& SrcPinValueType);

	void ChangePinCategoryToDouble(FEdGraphPinType& PinType);

	void ChangePinCategoryToWeightOrProbEntry(FEdGraphPinType& PinType);

	void ChangePinCategoryToWildcard(FEdGraphPinType& PinType);

	void ChangePinCategoryToName(FEdGraphPinType& PinType);

	void ChangePinValueCategoryToDouble(FEdGraphTerminalType& PinValueType);

	void ChangePinValueCategoryToWeightOrProbEntry(FEdGraphTerminalType& PinValueType);

	void ChangePinTypeToDoubleArray(FEdGraphPinType& PinType);

	void ChangePinTypeToWeightOrProbArray(FEdGraphPinType& PinType);

	void ChangePinTypeToDoubleMap(FEdGraphPinType& PinType);

	void ChangePinTypeToWeightOrProbMap(FEdGraphPinType& PinType);
	
	void ChangePinTypeToDataTable(FEdGraphPinType& PinType);

	void ChangePinTypeToCookedSelectorDistribution(FEdGraphPinType& PinType);

	FText GetCurrentTooltip() const;

	UEdGraphPin* GetDataTypePin();

	UEdGraphPin* GetFormatPin();

	UEdGraphPin* GetInputPin();

	UEdGraphPin* GetInputDataTableWeightOrProbNamePin();
	
	UEdGraphPin* GetInputDataTableIsProbNamePin();

	UEdGraphPin* GetOutputPin();

	UEdGraphPin* GetOutputKeysPin();

	FNodeTextCache CachedToolTip;

	UPROPERTY()  // Need to store this in asset, plus need to use this in ExpandNode for the temperorary node copy.
	EFenixCookSelectorInputDataType CurrentDataType = EFenixCookSelectorInputDataType::Weight;

	UPROPERTY()  // Need to store this in asset, plus need to use this in ExpandNode for the temperorary node copy.
	EFenixCookSelectorInputFormat CurrentFormat = EFenixCookSelectorInputFormat::Array;

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
