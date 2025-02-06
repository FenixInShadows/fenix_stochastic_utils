// Copyright 2025, Tiannan Chen, All rights reserved.


#include "K2Node_CookSelectorInput.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Engine/DataTable.h"
#include "SelectorUtils.h"

#define PIN_NAME_DATA_TYPE (TEXT("DataType"))
#define PIN_NAME_FORMAT (TEXT("Format"))
#define PIN_NAME_WEIGHTS (TEXT("Weights"))
#define PIN_NAME_PROBS (TEXT("Probs"))
#define PIN_NAME_WEIGHT_OR_PROBS (TEXT("WeightOrProbs"))
#define PIN_NAME_WEIGHT_MAP (TEXT("WeightMap"))
#define PIN_NAME_PROB_MAP (TEXT("ProbMap"))
#define PIN_NAME_WEIGHT_OR_PROB_MAP (TEXT("WeightOrProbMap"))
#define PIN_NAME_DATA_TABLE (TEXT("DataTable"))
#define PIN_NAME_WEIGHT_PROPERTY_NAME (TEXT("WeightPropertyName"))
#define PIN_NAME_PROB_PROPERTY_NAME (TEXT("ProbPropertyName"))
#define PIN_NAME_WEIGHT_OR_PROB_PROPERTY_NAME (TEXT("WeightOrProbPropertyName"))
#define PIN_NAME_IS_PROB_PROPERTY_NAME (TEXT("IsProbPropertyName"))
#define PIN_NAME_CUM_WEIGHTS (TEXT("CumWeights"))
#define PIN_NAME_CUM_PROBS (TEXT("CumProbs"))
#define PIN_NAME_COOKED_DISTRIBUTION (TEXT("CookedDistribution"))
#define PIN_NAME_KEYS (TEXT("Keys"))
#define PIN_NAME_ROW_NAMES (TEXT("RowNames"))

FText UK2Node_CookSelectorInput::GetTooltipText() const
{
	if (CachedToolTip.IsOutOfDate(this))
	{
		CachedToolTip.SetCachedText(GetCurrentTooltip(), this);
	}
	return CachedToolTip;
}

void UK2Node_CookSelectorInput::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegister) const
{
	UClass* ActionKey = GetClass();
	if (ActionRegister.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner);
		ActionRegister.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FNodeHandlingFunctor* UK2Node_CookSelectorInput::CreateNodeHandler(FKismetCompilerContext& CompilerContext) const
{
	return new FNodeHandlingFunctor(CompilerContext);
}

void UK2Node_CookSelectorInput::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	/*FName FunctionName;
	FString FuncInputPinName;

	switch (CurrentInputFormat)
	{
	case EFenixCookSelectorInputFormat::Basic:
		FunctionName = GET_FUNCTION_NAME_CHECKED(UK2Node_CookSelectorInput, TestK2Basic);
		FuncInputPinName = "InputInt";
		break;
	case EFenixCookSelectorInputFormat::Array:
		FunctionName = GET_FUNCTION_NAME_CHECKED(UK2Node_CookSelectorInput, TestK2Array);
		FuncInputPinName = "InputArray";
		break;
	case EFenixCookSelectorInputFormat::Set:
		FunctionName = GET_FUNCTION_NAME_CHECKED(UK2Node_CookSelectorInput, TestK2Set);
		FuncInputPinName = "InputSet";
		break;
	case EFenixCookSelectorInputFormat::Map:
		FunctionName = GET_FUNCTION_NAME_CHECKED(UK2Node_CookSelectorInput, TestK2Map);
		FuncInputPinName = "InputMap";
		break;
	default:
		BreakAllNodeLinks();
		return;
	}

	UK2Node_CallFunction* CallFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallFuncNode->FunctionReference.SetExternalMember(FunctionName, UK2Node_CookSelectorInput::StaticClass());
	CallFuncNode->AllocateDefaultPins();

	CompilerContext.MovePinLinksToIntermediate(*(GetInputPin()), *(CallFuncNode->FindPin(FuncInputPinName)));
	CompilerContext.MovePinLinksToIntermediate(*(GetOutputPin()), *(CallFuncNode->GetReturnValuePin()));*/

	FName FuncName;
	FString FuncInputPinName;
	FString FuncOutputPinName;

	switch (CurrentDataType)
	{
	case EFenixCookSelectorInputDataType::Weight:
		FuncName = GET_FUNCTION_NAME_CHECKED(USelectorUtils, MakeCumulatives);
		FuncInputPinName = "Values";
		FuncOutputPinName = "OutCumulatives";
		break;
	case EFenixCookSelectorInputDataType::Prob:
		FuncName = GET_FUNCTION_NAME_CHECKED(USelectorUtils, MakeCumulativesWithCutoff);
		FuncInputPinName = "Values";
		FuncOutputPinName = "OutCumulatives";
		break;
	case EFenixCookSelectorInputDataType::WeightOrProb:
		FuncName = GET_FUNCTION_NAME_CHECKED(USelectorUtils, CookSelectorDistribution);
		FuncInputPinName = "Entries";
		FuncOutputPinName = "OutDistribution";
		break;
	default:  // should not happen
		BreakAllNodeLinks();
		return;
	}
	
	UK2Node_CallFunction* CallFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallFuncNode->FunctionReference.SetExternalMember(FuncName, USelectorUtils::StaticClass());
	CallFuncNode->AllocateDefaultPins();

	CompilerContext.MovePinLinksToIntermediate(*(GetInputPin()), *(CallFuncNode->FindPin(FuncInputPinName)));
	CompilerContext.MovePinLinksToIntermediate(*(GetOutputPin()), *(CallFuncNode->FindPin(FuncOutputPinName)));

	BreakAllNodeLinks();
}

void UK2Node_CookSelectorInput::AllocateDefaultPins()
{
	UEnum* DataTypeTypeObject = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EFenixCookSelectorInputDataType"), true);
	UEdGraphPin* DataTypePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, DataTypeTypeObject, PIN_NAME_DATA_TYPE);
	DataTypePin->DefaultValue = DataTypeTypeObject->GetNameStringByValue(static_cast<int64>(CurrentDataType));

	UEnum* FormatTypeObject = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EFenixCookSelectorInputFormat"), true);
	UEdGraphPin* FormatPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, FormatTypeObject, PIN_NAME_FORMAT);
	FormatPin->DefaultValue = FormatTypeObject->GetNameStringByValue(static_cast<int64>(CurrentFormat));

	CreateInOutPins();

	Super::AllocateDefaultPins();
}

void UK2Node_CookSelectorInput::PinDefaultValueChanged(UEdGraphPin* ChangedPin)
{
	Super::PinDefaultValueChanged(ChangedPin);

	if (ChangedPin == GetDataTypePin())
	{
		UEnum* DataTypeTypeObject = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EFenixCookSelectorInputDataType"), true);
		const EFenixCookSelectorInputDataType NewDataType = static_cast<EFenixCookSelectorInputDataType>(DataTypeTypeObject->GetValueByNameString(GetDataTypePin()->DefaultValue));
		OnDataTypePinUpdated(NewDataType);
	}
	else if (ChangedPin == GetFormatPin())
	{
		UEnum* FormatTypeObject = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EFenixCookSelectorInputFormat"), true);
		const EFenixCookSelectorInputFormat NewFormat = static_cast<EFenixCookSelectorInputFormat>(FormatTypeObject->GetValueByNameString(GetFormatPin()->DefaultValue));
		OnFormatPinUpdated(NewFormat);
	}
}

void UK2Node_CookSelectorInput::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::PinConnectionListChanged(Pin);

	if (CurrentFormat == EFenixCookSelectorInputFormat::Map)
	{
		UEdGraphPin* InputPin = GetInputPin();
		UEdGraphPin* OutputKeysPin = GetOutputKeysPin();
		if (Pin == InputPin)
		{
			OnPinConnectionUpdateInMapFormat(InputPin, OutputKeysPin);
		}
		else if (Pin == OutputKeysPin)
		{
			OnPinConnectionUpdateInMapFormat(OutputKeysPin, InputPin);
		}
	}
}

void UK2Node_CookSelectorInput::PostReconstructNode()
{
	Super::PostReconstructNode();

	// Redo wildcard type-matching after reconstruction
	if (CurrentFormat == EFenixCookSelectorInputFormat::Map)
	{
		UEdGraphPin* InputPin = GetInputPin();
		UEdGraphPin* OutputKeysPin = GetOutputKeysPin();
		const bool updated = PostPinConnectionReconstructionInMapFormat(InputPin, OutputKeysPin);
		if (!updated)
		{
			PostPinConnectionReconstructionInMapFormat(OutputKeysPin, InputPin);
		}
	}
}

void UK2Node_CookSelectorInput::CreateInOutPins()
{
	// Input pins
	FCreatePinParams InPinParams;
	switch (CurrentFormat)
	{
	case EFenixCookSelectorInputFormat::Array:
		InPinParams.ContainerType = EPinContainerType::Array;
		switch (CurrentDataType)
		{
		case EFenixCookSelectorInputDataType::Weight:
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Real, UEdGraphSchema_K2::PC_Double, PIN_NAME_WEIGHTS, InPinParams);
			break;
		case EFenixCookSelectorInputDataType::Prob:
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Real, UEdGraphSchema_K2::PC_Double, PIN_NAME_PROBS, InPinParams);
			break;
		case EFenixCookSelectorInputDataType::WeightOrProb:
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FWeightOrProbEntry::StaticStruct(), PIN_NAME_WEIGHT_OR_PROBS, InPinParams);
			break;
		}
		break;
	case EFenixCookSelectorInputFormat::Map:
		InPinParams.ContainerType = EPinContainerType::Map;		
		switch (CurrentDataType)
		{
		case EFenixCookSelectorInputDataType::Weight:
			InPinParams.ValueTerminalType.TerminalCategory = UEdGraphSchema_K2::PC_Real;
			InPinParams.ValueTerminalType.TerminalSubCategory = UEdGraphSchema_K2::PC_Double;
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, PIN_NAME_WEIGHT_MAP, InPinParams);
			break;
		case EFenixCookSelectorInputDataType::Prob:
			InPinParams.ValueTerminalType.TerminalCategory = UEdGraphSchema_K2::PC_Real;
			InPinParams.ValueTerminalType.TerminalSubCategory = UEdGraphSchema_K2::PC_Double;
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, PIN_NAME_PROB_MAP, InPinParams);
			break;
		case EFenixCookSelectorInputDataType::WeightOrProb:
			InPinParams.ValueTerminalType.TerminalCategory = UEdGraphSchema_K2::PC_Struct;
			InPinParams.ValueTerminalType.TerminalSubCategoryObject = FWeightOrProbEntry::StaticStruct();
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, PIN_NAME_WEIGHT_OR_PROB_MAP, InPinParams);
			break;
		}
		break;
	case EFenixCookSelectorInputFormat::DataTable:
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UDataTable::StaticClass(), PIN_NAME_DATA_TABLE);
		switch (CurrentDataType)
		{
		case EFenixCookSelectorInputDataType::Weight:
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_WEIGHT_PROPERTY_NAME);
			break;
		case EFenixCookSelectorInputDataType::Prob:
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_PROB_PROPERTY_NAME);
			break;
		case EFenixCookSelectorInputDataType::WeightOrProb:
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_WEIGHT_OR_PROB_PROPERTY_NAME);
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_IS_PROB_PROPERTY_NAME);
			break;
		}
		break;		
	}

	// Output pins
	FCreatePinParams OutPinParams;
	switch (CurrentDataType)
	{
	case EFenixCookSelectorInputDataType::Weight:
		OutPinParams.ContainerType = EPinContainerType::Array;
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Real, UEdGraphSchema_K2::PC_Double, PIN_NAME_CUM_WEIGHTS, OutPinParams);
		break;
	case EFenixCookSelectorInputDataType::Prob:
		OutPinParams.ContainerType = EPinContainerType::Array;
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Real, UEdGraphSchema_K2::PC_Double, PIN_NAME_CUM_PROBS, OutPinParams);
		break;
	case EFenixCookSelectorInputDataType::WeightOrProb:
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, FCookedSelectorDistribution::StaticStruct(), PIN_NAME_COOKED_DISTRIBUTION, OutPinParams);
		break;
	}
	FCreatePinParams OutKeysPinParam;
	OutKeysPinParam.ContainerType = EPinContainerType::Array;
	switch (CurrentFormat)
	{
	case EFenixCookSelectorInputFormat::Map:
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, PIN_NAME_KEYS, OutKeysPinParam);
		break;
	case EFenixCookSelectorInputFormat::DataTable:
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Name, PIN_NAME_ROW_NAMES, OutKeysPinParam);
		break;
	}
}

void UK2Node_CookSelectorInput::OnDataTypePinUpdated(const EFenixCookSelectorInputDataType NewDataType)
{
	// Input pins
	UEdGraphPin* InputPin = GetInputPin();
	UEdGraphPin* InputDataTableWeightOrProbNamePin = GetInputDataTableWeightOrProbNamePin();
	UEdGraphPin* InputDataTableIsProbNamePin = GetInputDataTableIsProbNamePin();
	switch (CurrentFormat)
	{
	case EFenixCookSelectorInputFormat::Array:
		switch (NewDataType)
		{
		case EFenixCookSelectorInputDataType::Weight:
			InputPin->PinName = PIN_NAME_WEIGHTS;
			ChangePinCategoryToDouble(InputPin->PinType);
			break;
		case EFenixCookSelectorInputDataType::Prob:
			InputPin->PinName = PIN_NAME_PROBS;
			ChangePinCategoryToDouble(InputPin->PinType);
			break;
		case EFenixCookSelectorInputDataType::WeightOrProb:
			InputPin->PinName = PIN_NAME_WEIGHT_OR_PROBS;
			ChangePinCategoryToWeightOrProbEntry(InputPin->PinType);
			break;
		}
		break;
	case EFenixCookSelectorInputFormat::Map:
		switch (NewDataType)
		{
		case EFenixCookSelectorInputDataType::Weight:
			InputPin->PinName = PIN_NAME_WEIGHT_MAP;
			ChangePinValueCategoryToDouble(InputPin->PinType.PinValueType);
			break;
		case EFenixCookSelectorInputDataType::Prob:
			InputPin->PinName = PIN_NAME_PROB_MAP;
			ChangePinValueCategoryToDouble(InputPin->PinType.PinValueType);
			break;
		case EFenixCookSelectorInputDataType::WeightOrProb:
			InputPin->PinName = PIN_NAME_WEIGHT_OR_PROB_MAP;
			ChangePinValueCategoryToWeightOrProbEntry(InputPin->PinType.PinValueType);
			break;
		}
		break;
	case EFenixCookSelectorInputFormat::DataTable:
		switch (NewDataType)
		{
		case EFenixCookSelectorInputDataType::Weight:
			InputDataTableWeightOrProbNamePin->PinName = PIN_NAME_WEIGHT_PROPERTY_NAME;
			if (InputDataTableIsProbNamePin)
			{
				RemovePin(InputDataTableIsProbNamePin);
			}
			break;
		case EFenixCookSelectorInputDataType::Prob:
			InputDataTableWeightOrProbNamePin->PinName = PIN_NAME_PROB_PROPERTY_NAME;
			if (InputDataTableIsProbNamePin)
			{
				RemovePin(InputDataTableIsProbNamePin);
			}
			break;
		case EFenixCookSelectorInputDataType::WeightOrProb:
			InputDataTableWeightOrProbNamePin->PinName = PIN_NAME_WEIGHT_OR_PROB_PROPERTY_NAME;
			if (!InputDataTableIsProbNamePin)
			{
				CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_IS_PROB_PROPERTY_NAME);
			}
			break;
		}
		break;
	}

	// Output pins
	UEdGraphPin* OutputPin = GetOutputPin();
	switch (NewDataType)
	{
	case EFenixCookSelectorInputDataType::Weight:
		if (!OutputPin->SubPins.IsEmpty())
		{
			GetSchema()->RecombinePin(OutputPin->SubPins[0]);
		}
		OutputPin->PinName = PIN_NAME_CUM_WEIGHTS;
		ChangePinTypeToDoubleArray(OutputPin->PinType);
		break;
	case EFenixCookSelectorInputDataType::Prob:
		if (!OutputPin->SubPins.IsEmpty())
		{
			GetSchema()->RecombinePin(OutputPin->SubPins[0]);
		}
		OutputPin->PinName = PIN_NAME_CUM_PROBS;
		ChangePinTypeToDoubleArray(OutputPin->PinType);
		break;
	case EFenixCookSelectorInputDataType::WeightOrProb:
		OutputPin->PinName = PIN_NAME_COOKED_DISTRIBUTION;
		ChangePinTypeToCookedSelectorDistribution(OutputPin->PinType);
		break;
	}

	// Update data type cache
	CurrentDataType = NewDataType;

	// Mark dirty/modified
	CachedToolTip.MarkDirty();
	FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());
	GetGraph()->NotifyGraphChanged();

	// Refresh type related neighbor nodes
	if (!InputPin->LinkedTo.IsEmpty())
	{
		InputPin->LinkedTo[0]->GetOwningNode()->ReconstructNode();
	}
	if (!OutputPin->LinkedTo.IsEmpty())
	{
		OutputPin->LinkedTo[0]->GetOwningNode()->ReconstructNode();
	}
}

void UK2Node_CookSelectorInput::OnFormatPinUpdated(const EFenixCookSelectorInputFormat NewFormat)
{
	// Input pins
	UEdGraphPin* InputPin = GetInputPin();
	UEdGraphPin* InputDataTableWeightOrProbNamePin = GetInputDataTableWeightOrProbNamePin();
	UEdGraphPin* InputDataTableIsProbNamePin = GetInputDataTableIsProbNamePin();
	switch (NewFormat)
	{
	case EFenixCookSelectorInputFormat::Array:
		switch (CurrentDataType)
		{
		case EFenixCookSelectorInputDataType::Weight:
			InputPin->PinName = PIN_NAME_WEIGHTS;
			ChangePinTypeToDoubleArray(InputPin->PinType);
			break;
		case EFenixCookSelectorInputDataType::Prob:
			InputPin->PinName = PIN_NAME_PROBS;
			ChangePinTypeToDoubleArray(InputPin->PinType);
			break;
		case EFenixCookSelectorInputDataType::WeightOrProb:
			InputPin->PinName = PIN_NAME_WEIGHT_OR_PROBS;
			ChangePinTypeToWeightOrProbArray(InputPin->PinType);
			break;
		}
		if (InputDataTableWeightOrProbNamePin)
		{
			RemovePin(InputDataTableWeightOrProbNamePin);
		}
		if (InputDataTableIsProbNamePin)
		{
			RemovePin(InputDataTableIsProbNamePin);
		}
		break;
	case EFenixCookSelectorInputFormat::Map:
		switch (CurrentDataType)
		{
		case EFenixCookSelectorInputDataType::Weight:
			InputPin->PinName = PIN_NAME_WEIGHT_MAP;
			ChangePinTypeToDoubleMap(InputPin->PinType);
			break;
		case EFenixCookSelectorInputDataType::Prob:
			InputPin->PinName = PIN_NAME_PROB_MAP;
			ChangePinTypeToDoubleMap(InputPin->PinType);
			break;
		case EFenixCookSelectorInputDataType::WeightOrProb:
			InputPin->PinName = PIN_NAME_WEIGHT_OR_PROB_MAP;
			ChangePinTypeToWeightOrProbMap(InputPin->PinType);
			break;
		}
		if (InputDataTableWeightOrProbNamePin)
		{
			RemovePin(InputDataTableWeightOrProbNamePin);
		}
		if (InputDataTableIsProbNamePin)
		{
			RemovePin(InputDataTableIsProbNamePin);
		}
		break;
	case EFenixCookSelectorInputFormat::DataTable:
		InputPin->PinName = PIN_NAME_DATA_TABLE;
		ChangePinTypeToDataTable(InputPin->PinType);
		switch (CurrentDataType)
		{
		case EFenixCookSelectorInputDataType::Weight:
			if (!InputDataTableWeightOrProbNamePin)
			{
				CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_WEIGHT_PROPERTY_NAME);
			}
			break;
		case EFenixCookSelectorInputDataType::Prob:
			if (!InputDataTableWeightOrProbNamePin)
			{
				CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_PROB_PROPERTY_NAME);
			}
			break;
		case EFenixCookSelectorInputDataType::WeightOrProb:
			if (!InputDataTableWeightOrProbNamePin)
			{
				CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_WEIGHT_OR_PROB_PROPERTY_NAME);
			}
			if (!InputDataTableIsProbNamePin)
			{
				CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_IS_PROB_PROPERTY_NAME);
			}
			break;
		}
		break;
	}

	// Output pins
	UEdGraphPin* OutputKeysPin = GetOutputKeysPin();
	FCreatePinParams OutKeysPinParams;
	OutKeysPinParams.ContainerType = EPinContainerType::Array;
	switch (NewFormat)
	{
	case EFenixCookSelectorInputFormat::Array:
		if (OutputKeysPin)
		{
			RemovePin(OutputKeysPin);
		}
		break;
	case EFenixCookSelectorInputFormat::Map:
		if (!OutputKeysPin)
		{
			CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, PIN_NAME_KEYS, OutKeysPinParams);
		}
		else
		{
			OutputKeysPin->PinName = PIN_NAME_KEYS;
			if (OutputKeysPin->LinkedTo.IsEmpty())
			{
				ChangePinCategoryToWildcard(OutputKeysPin->PinType);
			}
		}
		break;
	case EFenixCookSelectorInputFormat::DataTable:
		if (!OutputKeysPin)
		{
			CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Name, PIN_NAME_ROW_NAMES, OutKeysPinParams);
		}
		else
		{
			OutputKeysPin->PinName = PIN_NAME_ROW_NAMES;
			ChangePinCategoryToName(OutputKeysPin->PinType);
		}
		break;
	}
	
	// Update format cache
	CurrentFormat = NewFormat;

	// Mark dirty/modified
	CachedToolTip.MarkDirty();
	FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());
	GetGraph()->NotifyGraphChanged();

	// Refresh type related neighbor nodes
	if (!InputPin->LinkedTo.IsEmpty())
	{
		InputPin->LinkedTo[0]->GetOwningNode()->ReconstructNode();
	}
	UEdGraphPin* NewOutputKeysPin = GetOutputKeysPin();
	if (NewOutputKeysPin && !NewOutputKeysPin->LinkedTo.IsEmpty())
	{
		NewOutputKeysPin->LinkedTo[0]->GetOwningNode()->ReconstructNode();
	}

	// For map format, also refresh this node
	if (CurrentFormat == EFenixCookSelectorInputFormat::Map)
	{
		ReconstructNode();
	}
}

void UK2Node_CookSelectorInput::OnPinConnectionUpdateInMapFormat(UEdGraphPin* Pin, UEdGraphPin* SyncedPin)
{
	FEdGraphPinType& PinType = Pin->PinType;
	FEdGraphPinType& SyncedPinType = SyncedPin->PinType;
	if (Pin->LinkedTo.IsEmpty())
	{
		if (SyncedPin->LinkedTo.IsEmpty())
		{
			ChangeToWildCardPinType(PinType);
			ChangeToWildCardPinType(SyncedPinType);
		}
	}
	else
	{
		UEdGraphPin* NeighborPin = Pin->LinkedTo[0];
		const FEdGraphPinType& NeighborPinType = NeighborPin->PinType;
		if (NeighborPinType.PinCategory != UEdGraphSchema_K2::PC_Wildcard)
		{
			CopyPinTypeCategoryInfo(PinType, NeighborPinType);
			CopyPinTypeCategoryInfo(SyncedPinType, NeighborPinType);
			if (!SyncedPin->LinkedTo.IsEmpty())
			{
				SyncedPin->LinkedTo[0]->GetOwningNode()->ReconstructNode();
			}
		}
	}
}

bool UK2Node_CookSelectorInput::PostPinConnectionReconstructionInMapFormat(UEdGraphPin* Pin, UEdGraphPin* SyncedPin)
{
	if (!Pin->LinkedTo.IsEmpty())
	{
		UEdGraphPin* NeighborPin = Pin->LinkedTo[0];
		const FEdGraphPinType& NeighborPinType = NeighborPin->PinType;
		if (NeighborPinType.PinCategory != UEdGraphSchema_K2::PC_Wildcard)
		{
			CopyPinTypeCategoryInfo(Pin->PinType, NeighborPinType);
			CopyPinTypeCategoryInfo(SyncedPin->PinType, NeighborPinType);
			if (!SyncedPin->LinkedTo.IsEmpty())
			{
				SyncedPin->LinkedTo[0]->GetOwningNode()->ReconstructNode();
			}
			return true;
		}
	}

	return false;
}

void UK2Node_CookSelectorInput::ChangeToWildCardPinType(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = nullptr;
	PinType.bIsReference = false;
	PinType.bIsConst = false;
	PinType.bIsWeakPointer = false;
	PinType.bIsUObjectWrapper = false;
	PinType.bSerializeAsSinglePrecisionFloat = false;
}

void UK2Node_CookSelectorInput::CopyPinTypeCategoryInfo(FEdGraphPinType& PinType, const FEdGraphPinType& SrcPinType)
{
	PinType.PinCategory = SrcPinType.PinCategory;
	PinType.PinSubCategory = SrcPinType.PinSubCategory;
	PinType.PinSubCategoryObject = SrcPinType.PinSubCategoryObject;
	PinType.bIsReference = SrcPinType.bIsReference;
	PinType.bIsConst = SrcPinType.bIsConst;
	PinType.bIsWeakPointer = SrcPinType.bIsWeakPointer;
	PinType.bIsUObjectWrapper = SrcPinType.bIsUObjectWrapper;
	PinType.bSerializeAsSinglePrecisionFloat = SrcPinType.bSerializeAsSinglePrecisionFloat;
}

void UK2Node_CookSelectorInput::ChangePinCategoryToDouble(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Real;
	PinType.PinSubCategory = UEdGraphSchema_K2::PC_Double;
	PinType.PinSubCategoryObject = nullptr;
}

void UK2Node_CookSelectorInput::ChangePinCategoryToWeightOrProbEntry(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = FWeightOrProbEntry::StaticStruct();
}

void UK2Node_CookSelectorInput::ChangePinCategoryToWildcard(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = nullptr;
}

void UK2Node_CookSelectorInput::ChangePinCategoryToName(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Name;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = nullptr;
}

void UK2Node_CookSelectorInput::ChangePinValueCategoryToDouble(FEdGraphTerminalType& TerminalType)
{
	TerminalType.TerminalCategory = UEdGraphSchema_K2::PC_Real;
	TerminalType.TerminalSubCategory = UEdGraphSchema_K2::PC_Double;
	TerminalType.TerminalSubCategoryObject = nullptr;
}

void UK2Node_CookSelectorInput::ChangePinValueCategoryToWeightOrProbEntry(FEdGraphTerminalType& TerminalType)
{
	TerminalType.TerminalCategory = UEdGraphSchema_K2::PC_Struct;
	TerminalType.TerminalSubCategory = NAME_None;
	TerminalType.TerminalSubCategoryObject = FWeightOrProbEntry::StaticStruct();
}

void UK2Node_CookSelectorInput::ChangePinTypeToDoubleArray(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Real;
	PinType.PinSubCategory = UEdGraphSchema_K2::PC_Double;
	PinType.PinSubCategoryObject = nullptr;
	PinType.ContainerType = EPinContainerType::Array;
	PinType.PinValueType = FEdGraphTerminalType();
}

void UK2Node_CookSelectorInput::ChangePinTypeToWeightOrProbArray(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = FWeightOrProbEntry::StaticStruct();
	PinType.ContainerType = EPinContainerType::Array;
	PinType.PinValueType = FEdGraphTerminalType();
}

void UK2Node_CookSelectorInput::ChangePinTypeToDoubleMap(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = nullptr;
	PinType.ContainerType = EPinContainerType::Map;
	FEdGraphTerminalType ValueType;
	ValueType.TerminalCategory = UEdGraphSchema_K2::PC_Real;
	ValueType.TerminalSubCategory = UEdGraphSchema_K2::PC_Double;
	PinType.PinValueType = ValueType;
}

void UK2Node_CookSelectorInput::ChangePinTypeToWeightOrProbMap(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = nullptr;
	PinType.ContainerType = EPinContainerType::Map;
	FEdGraphTerminalType ValueType;
	ValueType.TerminalCategory = UEdGraphSchema_K2::PC_Struct;
	ValueType.TerminalSubCategoryObject = FWeightOrProbEntry::StaticStruct();
	PinType.PinValueType = ValueType;
}

void UK2Node_CookSelectorInput::ChangePinTypeToDataTable(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Object;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = UDataTable::StaticClass();
	PinType.ContainerType = EPinContainerType::None;
	PinType.PinValueType = FEdGraphTerminalType();
}

void UK2Node_CookSelectorInput::ChangePinTypeToCookedSelectorDistribution(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = FCookedSelectorDistribution::StaticStruct();
	PinType.ContainerType = EPinContainerType::None;
	PinType.PinValueType = FEdGraphTerminalType();
}

FText UK2Node_CookSelectorInput::GetCurrentTooltip() const
{
	FText FormatString = FText::FromString("{0} {1}.");
	switch (CurrentDataType)
	{
	case EFenixCookSelectorInputDataType::Weight:
		FormatString = FText::FromString("Cook a weight {0} into {1}cumulative weights.");
		break;
	case EFenixCookSelectorInputDataType::Prob:
		FormatString = FText::FromString("Cook a probability {0} into {1}cumulative probabilities.");
		break;
	case EFenixCookSelectorInputDataType::WeightOrProb:
		FormatString = FText::FromString("Cook a \"weight or probability\" {0} into {1}a CookedSelectorDistribution.");
		break;
	}

	FText Arg0 = FText();
	FText Arg1 = FText();
	switch (CurrentFormat)
	{
	case EFenixCookSelectorInputFormat::Array:
		Arg0 = FText::FromString("array");
		break;
	case EFenixCookSelectorInputFormat::Map:
		Arg0 = FText::FromString("map");
		Arg1 = FText::FromString("keys and ");
		break;
	case EFenixCookSelectorInputFormat::DataTable:
		Arg0 = FText::FromString("data table");
		Arg1 = FText::FromString("row names and ");
		break;
	}

	return FText::Format(FormatString, Arg0, Arg1);
}

UEdGraphPin* UK2Node_CookSelectorInput::GetDataTypePin()
{
	return FindPin(PIN_NAME_DATA_TYPE);
}

UEdGraphPin* UK2Node_CookSelectorInput::GetFormatPin()
{
	return FindPin(PIN_NAME_FORMAT);
}

UEdGraphPin* UK2Node_CookSelectorInput::GetInputPin()
{
	switch (CurrentFormat)
	{
	case EFenixCookSelectorInputFormat::Array:
		switch (CurrentDataType)
		{
		case EFenixCookSelectorInputDataType::Weight:
			return FindPin(PIN_NAME_WEIGHTS);
		case EFenixCookSelectorInputDataType::Prob:
			return FindPin(PIN_NAME_PROBS);
		case EFenixCookSelectorInputDataType::WeightOrProb:
			return FindPin(PIN_NAME_WEIGHT_OR_PROBS);
		}
		break;
	case EFenixCookSelectorInputFormat::Map:
		switch (CurrentDataType)
		{
		case EFenixCookSelectorInputDataType::Weight:
			return FindPin(PIN_NAME_WEIGHT_MAP);
		case EFenixCookSelectorInputDataType::Prob:
			return FindPin(PIN_NAME_PROB_MAP);
		case EFenixCookSelectorInputDataType::WeightOrProb:
			return FindPin(PIN_NAME_WEIGHT_OR_PROB_MAP);
		}
		break;
	case EFenixCookSelectorInputFormat::DataTable:
		return FindPin(PIN_NAME_DATA_TABLE);
	}
	return nullptr;
}

UEdGraphPin* UK2Node_CookSelectorInput::GetInputDataTableWeightOrProbNamePin()
{
	switch (CurrentDataType)
	{
	case EFenixCookSelectorInputDataType::Weight:
		return FindPin(PIN_NAME_WEIGHT_PROPERTY_NAME);
	case EFenixCookSelectorInputDataType::Prob:
		return FindPin(PIN_NAME_PROB_PROPERTY_NAME);
	case EFenixCookSelectorInputDataType::WeightOrProb:
		return FindPin(PIN_NAME_WEIGHT_OR_PROB_PROPERTY_NAME);
	}
	return nullptr;
}

UEdGraphPin* UK2Node_CookSelectorInput::GetInputDataTableIsProbNamePin()
{
	return FindPin(PIN_NAME_IS_PROB_PROPERTY_NAME);
}

UEdGraphPin* UK2Node_CookSelectorInput::GetOutputPin()
{
	switch (CurrentDataType)
	{
	case EFenixCookSelectorInputDataType::Weight:
		return FindPin(PIN_NAME_CUM_WEIGHTS);
	case EFenixCookSelectorInputDataType::Prob:
		return FindPin(PIN_NAME_CUM_PROBS);
	case EFenixCookSelectorInputDataType::WeightOrProb:
		return FindPin(PIN_NAME_COOKED_DISTRIBUTION);
	}
	return nullptr;
}

UEdGraphPin* UK2Node_CookSelectorInput::GetOutputKeysPin()
{
	switch (CurrentFormat)
	{
	case EFenixCookSelectorInputFormat::Array:
		return nullptr;
	case EFenixCookSelectorInputFormat::Map:
		return FindPin(PIN_NAME_KEYS);
	case EFenixCookSelectorInputFormat::DataTable:
		return FindPin(PIN_NAME_ROW_NAMES);
	}
	return nullptr;
}

int32 UK2Node_CookSelectorInput::TestK2Basic(const int32 InputInt)
{
	return InputInt + 1;
}

TArray<int32> UK2Node_CookSelectorInput::TestK2Array(const TArray<int32> InputArray)
{
	TArray<int> OutputArray = InputArray;
	OutputArray.Add(0);
	return OutputArray;
}

TSet<int32> UK2Node_CookSelectorInput::TestK2Set(const TSet<int32> InputSet)
{
	return InputSet.Union(TSet<int>{0});
}

TMap<int32, FString> UK2Node_CookSelectorInput::TestK2Map(const TMap<int32, FString> InputMap)
{
	TMap<int32, FString> OutputMap = InputMap;
	OutputMap.Add(0, "DefaultValue");
	return OutputMap;
}
