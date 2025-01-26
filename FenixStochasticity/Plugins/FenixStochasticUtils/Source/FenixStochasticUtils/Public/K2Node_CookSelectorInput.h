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

	virtual bool IsNodePure() const override { return true; }

	virtual bool IsNodeSafeToIgnore() const override { return true; }

	virtual FNodeHandlingFunctor* CreateNodeHandler(FKismetCompilerContext& CompilerContext) const override;

	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

	virtual void AllocateDefaultPins() override;
	
	virtual void PinDefaultValueChanged(UEdGraphPin* ChangedPin) override;

	void CreateInOutPins();

	void OnDataTypePinUpdated(const EFenixCookSelectorInputDataType NewDataType);

	void OnFormatPinUpdated(const EFenixCookSelectorInputFormat NewFormat);

	FText GetCurrentTooltip() const;

	UEdGraphPin* GetDataTypePin();

	UEdGraphPin* GetFormatPin();

	UEdGraphPin* GetInputPin();

	UEdGraphPin* GetInputDataTableWeightOrProbNamePin();
	
	UEdGraphPin* GetInputDataTableIsProbNamePin();

	UEdGraphPin* GetOutputPin();

	UEdGraphPin* GetOutputKeysPin();

	UFUNCTION(BlueprintPure, meta = (BlueprintInternalUseOnly = "true"))
	static int32 TestK2Basic(const int32 InputInt);

	UFUNCTION(BlueprintPure, meta = (BlueprintInternalUseOnly = "true"))
	static TArray<int32> TestK2Array(const TArray<int32> InputArray);

	UFUNCTION(BlueprintPure, meta = (BlueprintInternalUseOnly = "true"))
	static TSet<int32> TestK2Set(const TSet<int32> InputSet);

	UFUNCTION(BlueprintPure, meta = (BlueprintInternalUseOnly = "true"))
	static TMap<int32, FString> TestK2Map(const TMap<int32, FString> InputMap);

	UPROPERTY()
	EFenixCookSelectorInputDataType CurrentDataType = EFenixCookSelectorInputDataType::Weight;

	UPROPERTY()
	EFenixCookSelectorInputFormat CurrentFormat = EFenixCookSelectorInputFormat::Array;

	FNodeTextCache CachedToolTip;
};
