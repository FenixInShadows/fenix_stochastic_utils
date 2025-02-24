// Copyright 2025, Tiannan Chen, All rights reserved.


#include "K2Node_CookSelectorInput.h"
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

void UK2Node_CookSelectorInput::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	FName FuncName;
	FString FuncInputPinName;
	FString FuncOutputPinName;

	switch (CurrentDataType)
	{
	case EFenixSelectorInputDataType::Weight:
		FuncName = GET_FUNCTION_NAME_CHECKED(USelectorUtils, MakeCumulatives);
		FuncInputPinName = "Values";
		FuncOutputPinName = "OutCumulatives";
		break;
	case EFenixSelectorInputDataType::Prob:
		FuncName = GET_FUNCTION_NAME_CHECKED(USelectorUtils, MakeCumulatives);  // Note: don't do cut off here, as we do not make much assumptions on how the result would be used
		FuncInputPinName = "Values";
		FuncOutputPinName = "OutCumulatives";
		break;
	case EFenixSelectorInputDataType::WeightOrProb:
		FuncName = GET_FUNCTION_NAME_CHECKED(USelectorUtils, CookSelectorDistribution);
		FuncInputPinName = "Entries";
		FuncOutputPinName = "OutDistribution";
		break;
	default:  // should not happen
		BreakAllNodeLinks();
		return;
	}
	
	UK2Node_CallFunction* CookFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CookFuncNode->FunctionReference.SetExternalMember(FuncName, USelectorUtils::StaticClass());
	CookFuncNode->AllocateDefaultPins();

	UEdGraphPin* ExecPin = GetExecPin();
	UEdGraphPin* InputPin = GetInputPin();
	UEdGraphPin* OutputPin = GetOutputPin();
	UEdGraphPin* ThenPin = GetThenPin();
	UEdGraphPin* CookFuncExecPin = CookFuncNode->GetExecPin();
	UEdGraphPin* CookFuncInputPin = CookFuncNode->FindPin(FuncInputPinName);
	UEdGraphPin* CookFuncOutputPin = CookFuncNode->FindPin(FuncOutputPinName);
	UEdGraphPin* CookFuncThenPin = CookFuncNode->GetThenPin();

	switch (CurrentFormat)
	{
	case EFenixSelectorInputFormat::Array:
		{
			CompilerContext.MovePinLinksToIntermediate(*ExecPin, *CookFuncExecPin);
			CompilerContext.MovePinLinksToIntermediate(*InputPin, *CookFuncInputPin);
			CompilerContext.MovePinLinksToIntermediate(*OutputPin, *CookFuncOutputPin);
			CompilerContext.MovePinLinksToIntermediate(*ThenPin, *CookFuncThenPin);
		}
		break;
	case EFenixSelectorInputFormat::Map:
		{
			FuncName = GET_FUNCTION_NAME_CHECKED(UBlueprintMapLibrary, Map_Keys);
			UK2Node_CallFunction* GetKeysFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
			GetKeysFuncNode->FunctionReference.SetExternalMember(FuncName, UBlueprintMapLibrary::StaticClass());
			GetKeysFuncNode->AllocateDefaultPins();
			UEdGraphPin* GetKeysInputPin = GetKeysFuncNode->FindPin(TEXT("TargetMap"));
			CommonEditorUtils::CopyPinTypeAndValueTypeInfo(GetKeysInputPin->PinType, InputPin->PinType);
			UEdGraphPin* GetKeysOutputPin = GetKeysFuncNode->FindPin(TEXT("Keys"));
			CommonEditorUtils::CopyPinTypeCategoryInfo(GetKeysOutputPin->PinType, InputPin->PinType);

			FuncName = GET_FUNCTION_NAME_CHECKED(UBlueprintMapLibrary, Map_Values);
			UK2Node_CallFunction* GetValuesFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
			GetValuesFuncNode->FunctionReference.SetExternalMember(FuncName, UBlueprintMapLibrary::StaticClass());
			GetValuesFuncNode->AllocateDefaultPins();
			UEdGraphPin* GetValuesInputPin = GetValuesFuncNode->FindPin(TEXT("TargetMap"));
			CommonEditorUtils::CopyPinTypeAndValueTypeInfo(GetValuesInputPin->PinType, InputPin->PinType);
			UEdGraphPin* GetValuesOutputPin = GetValuesFuncNode->FindPin(TEXT("Values"));
			CommonEditorUtils::CopyPinValueTypeToPinTypeInfo(GetValuesOutputPin->PinType, InputPin->PinType.PinValueType);

			// Keys = GetKeys(Map) -> Values = GetValues(Map) -> Cooked = Cook(Values) => Return (Cooked, Keys)
			CompilerContext.MovePinLinksToIntermediate(*ExecPin, *(GetKeysFuncNode->GetExecPin()));
			CompilerContext.CopyPinLinksToIntermediate(*InputPin, *GetKeysInputPin);
			CompilerContext.MovePinLinksToIntermediate(*GetOutputKeysPin(), *GetKeysOutputPin);
			GetKeysFuncNode->GetThenPin()->MakeLinkTo(GetValuesFuncNode->GetExecPin());
			CompilerContext.MovePinLinksToIntermediate(*InputPin, *GetValuesInputPin);
			GetValuesOutputPin->MakeLinkTo(CookFuncInputPin);
			GetValuesFuncNode->GetThenPin()->MakeLinkTo(CookFuncExecPin);
			CompilerContext.MovePinLinksToIntermediate(*OutputPin, *CookFuncOutputPin);
			CompilerContext.MovePinLinksToIntermediate(*ThenPin, *CookFuncThenPin);
		}
		break;
	case EFenixSelectorInputFormat::DataTable:
		FuncName = GET_FUNCTION_NAME_CHECKED(UDataTableFunctionLibrary, GetDataTableRowNames);
		UK2Node_CallFunction* GetKeysFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		GetKeysFuncNode->FunctionReference.SetExternalMember(FuncName, UDataTableFunctionLibrary::StaticClass());
		GetKeysFuncNode->AllocateDefaultPins();
		UEdGraphPin* GetKeysInputPin = GetKeysFuncNode->FindPin(TEXT("Table"));
		UEdGraphPin* GetKeysOutputPin = GetKeysFuncNode->FindPin(TEXT("OutRowNames"));

		UK2Node_CallFunction* GetValuesFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		UEdGraphPin* GetValuesInputPin = nullptr;
		UEdGraphPin* GetValuesWeightOrProbNamePin = nullptr;
		UEdGraphPin* GetValuesIsProbNamePin = nullptr;
		UEdGraphPin* GetValuesOutputPin = nullptr;
		switch (CurrentDataType)
		{
		case EFenixSelectorInputDataType::Weight:
		case EFenixSelectorInputDataType::Prob:
			FuncName = GET_FUNCTION_NAME_CHECKED(UCommonUtils, GetDataTableColumnAsFloats);
			GetValuesFuncNode->FunctionReference.SetExternalMember(FuncName, UCommonUtils::StaticClass());
			GetValuesFuncNode->AllocateDefaultPins();
			GetValuesInputPin = GetValuesFuncNode->FindPin(TEXT("DataTable"));
			GetValuesWeightOrProbNamePin = GetValuesFuncNode->FindPin(TEXT("PropertyName"));
			GetValuesOutputPin = GetValuesFuncNode->FindPin(TEXT("OutValues"));
			break;
		case EFenixSelectorInputDataType::WeightOrProb:
			FuncName = GET_FUNCTION_NAME_CHECKED(USelectorUtils, GetWeightOrProbEntriesFromDataTable);
			GetValuesFuncNode->FunctionReference.SetExternalMember(FuncName, USelectorUtils::StaticClass());
			GetValuesFuncNode->AllocateDefaultPins();
			GetValuesInputPin = GetValuesFuncNode->FindPin(TEXT("DataTable"));
			GetValuesWeightOrProbNamePin = GetValuesFuncNode->FindPin(TEXT("WeightOrProbPropertyName"));
			GetValuesIsProbNamePin = GetValuesFuncNode->FindPin(TEXT("IsProbPropertyName"));
			GetValuesOutputPin = GetValuesFuncNode->FindPin(TEXT("OutEntries"));
			break;
		}

		// Keys = GetRowNames(DataTable) -> Values = GetValues(DataTable, LabelNames) -> Cooked = Cook(Values) => Return (Cooked, Keys)
		CompilerContext.MovePinLinksToIntermediate(*ExecPin, *(GetKeysFuncNode->GetExecPin()));
		CompilerContext.CopyPinLinksToIntermediate(*InputPin, *GetKeysInputPin);
		CompilerContext.MovePinLinksToIntermediate(*GetOutputKeysPin(), *GetKeysOutputPin);
		GetKeysFuncNode->GetThenPin()->MakeLinkTo(GetValuesFuncNode->GetExecPin());
		CompilerContext.MovePinLinksToIntermediate(*InputPin, *GetValuesInputPin);
		CompilerContext.MovePinLinksToIntermediate(*GetInputDataTableWeightOrProbNamePin(), *GetValuesWeightOrProbNamePin);
		if (GetValuesIsProbNamePin)
		{
			CompilerContext.MovePinLinksToIntermediate(*GetInputDataTableIsProbNamePin(), *GetValuesIsProbNamePin);
		}
		GetValuesOutputPin->MakeLinkTo(CookFuncInputPin);
		GetValuesFuncNode->GetThenPin()->MakeLinkTo(CookFuncExecPin);
		CompilerContext.MovePinLinksToIntermediate(*OutputPin, *CookFuncOutputPin);
		CompilerContext.MovePinLinksToIntermediate(*ThenPin, *CookFuncThenPin);
		break;
	}

	BreakAllNodeLinks();
}

void UK2Node_CookSelectorInput::AllocateDefaultPins()
{
	// Add execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);

	// Add data type pin
	UEnum* DataTypeTypeObject = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EFenixSelectorInputDataType"), true);
	UEdGraphPin* DataTypePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, DataTypeTypeObject, PIN_NAME_DATA_TYPE);
	DataTypePin->DefaultValue = DataTypeTypeObject->GetNameStringByValue(static_cast<int64>(CurrentDataType));

	// Add format pin
	UEnum* FormatTypeObject = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EFenixSelectorInputFormat"), true);
	UEdGraphPin* FormatPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, FormatTypeObject, PIN_NAME_FORMAT);
	FormatPin->DefaultValue = FormatTypeObject->GetNameStringByValue(static_cast<int64>(CurrentFormat));

	// Add other pins
	CreateInOutPins();

	Super::AllocateDefaultPins();
}

void UK2Node_CookSelectorInput::PinDefaultValueChanged(UEdGraphPin* ChangedPin)
{
	Super::PinDefaultValueChanged(ChangedPin);

	if (ChangedPin == GetDataTypePin())
	{
		OnDataTypePinUpdated(ChangedPin);
	}
	else if (ChangedPin == GetFormatPin())
	{
		OnFormatPinUpdated(ChangedPin);
	}
	else if (ChangedPin == GetInputPin())
	{
		OnInputPinUpdated(ChangedPin);
	}
	else if (ChangedPin == GetInputDataTableWeightOrProbNamePin())
	{
		OnDataTableWeightOrProbNamePinUpdated(ChangedPin);
	}
	else if (ChangedPin == GetInputDataTableIsProbNamePin())
	{
		OnDataTableIsProbNamePinUpdated(ChangedPin);
	}
}

void UK2Node_CookSelectorInput::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::PinConnectionListChanged(Pin);

	if (CurrentFormat == EFenixSelectorInputFormat::Map)
	{
		UEdGraphPin* InputPin = GetInputPin();
		UEdGraphPin* OutputKeysPin = GetOutputKeysPin();
		if (Pin == InputPin)
		{
			CommonEditorUtils::OnPinConnectionUpdatedWithCategoryInfoSync(InputPin, OutputKeysPin);
		}
		else if (Pin == OutputKeysPin)
		{
			CommonEditorUtils::OnPinConnectionUpdatedWithCategoryInfoSync(OutputKeysPin, InputPin);
		}
	}
}

void UK2Node_CookSelectorInput::PostReconstructNode()
{
	Super::PostReconstructNode();

	// Redo wildcard type-matching after reconstruction
	if (CurrentFormat == EFenixSelectorInputFormat::Map)
	{
		UEdGraphPin* InputPin = GetInputPin();
		UEdGraphPin* OutputKeysPin = GetOutputKeysPin();
		const bool updated = CommonEditorUtils::PostPinConnectionReconstructionWithCategoryInfoSync(InputPin, OutputKeysPin);
		if (!updated)
		{
			CommonEditorUtils::PostPinConnectionReconstructionWithCategoryInfoSync(OutputKeysPin, InputPin);
		}
	}
}

void UK2Node_CookSelectorInput::CreateInOutPins()
{
	// Input pins
	FCreatePinParams InPinParams;
	UEdGraphPin* InputPin;
	UEdGraphPin* InputDataTableWeightOrProbNamePin;
	UEdGraphPin* InputDataTableIsProbNamePin;
	switch (CurrentFormat)
	{
	case EFenixSelectorInputFormat::Array:
		InPinParams.ContainerType = EPinContainerType::Array;
		switch (CurrentDataType)
		{
		case EFenixSelectorInputDataType::Weight:
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Real, UEdGraphSchema_K2::PC_Double, PIN_NAME_WEIGHTS, InPinParams);
			break;
		case EFenixSelectorInputDataType::Prob:
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Real, UEdGraphSchema_K2::PC_Double, PIN_NAME_PROBS, InPinParams);
			break;
		case EFenixSelectorInputDataType::WeightOrProb:
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FWeightOrProbEntry::StaticStruct(), PIN_NAME_WEIGHT_OR_PROBS, InPinParams);
			break;
		}
		break;
	case EFenixSelectorInputFormat::Map:
		InPinParams.ContainerType = EPinContainerType::Map;		
		switch (CurrentDataType)
		{
		case EFenixSelectorInputDataType::Weight:
			InPinParams.ValueTerminalType.TerminalCategory = UEdGraphSchema_K2::PC_Real;
			InPinParams.ValueTerminalType.TerminalSubCategory = UEdGraphSchema_K2::PC_Double;
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, PIN_NAME_WEIGHT_MAP, InPinParams);
			break;
		case EFenixSelectorInputDataType::Prob:
			InPinParams.ValueTerminalType.TerminalCategory = UEdGraphSchema_K2::PC_Real;
			InPinParams.ValueTerminalType.TerminalSubCategory = UEdGraphSchema_K2::PC_Double;
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, PIN_NAME_PROB_MAP, InPinParams);
			break;
		case EFenixSelectorInputDataType::WeightOrProb:
			InPinParams.ValueTerminalType.TerminalCategory = UEdGraphSchema_K2::PC_Struct;
			InPinParams.ValueTerminalType.TerminalSubCategoryObject = FWeightOrProbEntry::StaticStruct();
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, PIN_NAME_WEIGHT_OR_PROB_MAP, InPinParams);
			break;
		}
		break;
	case EFenixSelectorInputFormat::DataTable:
		InputPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UDataTable::StaticClass(), PIN_NAME_DATA_TABLE);
		InputPin->DefaultObject = DataTable;
		switch (CurrentDataType)
		{
		case EFenixSelectorInputDataType::Weight:
			InputDataTableWeightOrProbNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_WEIGHT_PROPERTY_NAME);
			InputDataTableWeightOrProbNamePin->DefaultValue = WeightPropertyName;
			break;
		case EFenixSelectorInputDataType::Prob:
			InputDataTableWeightOrProbNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_PROB_PROPERTY_NAME);
			InputDataTableWeightOrProbNamePin->DefaultValue = ProbPropertyName;
			break;
		case EFenixSelectorInputDataType::WeightOrProb:
			InputDataTableWeightOrProbNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_WEIGHT_OR_PROB_PROPERTY_NAME);
			InputDataTableWeightOrProbNamePin->DefaultValue = WeightOrProbPropertyName;
			InputDataTableIsProbNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_IS_PROB_PROPERTY_NAME);
			InputDataTableIsProbNamePin->DefaultValue = IsProbPropertyName;
			break;
		}
		break;		
	}

	// Output pins
	FCreatePinParams OutPinParams;
	switch (CurrentDataType)
	{
	case EFenixSelectorInputDataType::Weight:
		OutPinParams.ContainerType = EPinContainerType::Array;
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Real, UEdGraphSchema_K2::PC_Double, PIN_NAME_CUM_WEIGHTS, OutPinParams);
		break;
	case EFenixSelectorInputDataType::Prob:
		OutPinParams.ContainerType = EPinContainerType::Array;
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Real, UEdGraphSchema_K2::PC_Double, PIN_NAME_CUM_PROBS, OutPinParams);
		break;
	case EFenixSelectorInputDataType::WeightOrProb:
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, FCookedSelectorDistribution::StaticStruct(), PIN_NAME_COOKED_DISTRIBUTION, OutPinParams);
		break;
	}
	FCreatePinParams OutKeysPinParam;
	OutKeysPinParam.ContainerType = EPinContainerType::Array;
	switch (CurrentFormat)
	{
	case EFenixSelectorInputFormat::Map:
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, PIN_NAME_KEYS, OutKeysPinParam);
		break;
	case EFenixSelectorInputFormat::DataTable:
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Name, PIN_NAME_ROW_NAMES, OutKeysPinParam);
		break;
	}
}

void UK2Node_CookSelectorInput::OnDataTypePinUpdated(UEdGraphPin* ChangedPin)
{
	// Get new data type
	UEnum* DataTypeTypeObject = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EFenixSelectorInputDataType"), true);
	const EFenixSelectorInputDataType NewDataType = static_cast<EFenixSelectorInputDataType>(DataTypeTypeObject->GetValueByNameString(ChangedPin->DefaultValue));

	// Input pins
	UEdGraphPin* InputPin = GetInputPin();
	UEdGraphPin* InputDataTableWeightOrProbNamePin = GetInputDataTableWeightOrProbNamePin();
	UEdGraphPin* InputDataTableIsProbNamePin = GetInputDataTableIsProbNamePin();
	switch (CurrentFormat)
	{
	case EFenixSelectorInputFormat::Array:
		switch (NewDataType)
		{
		case EFenixSelectorInputDataType::Weight:
			InputPin->PinName = PIN_NAME_WEIGHTS;
			CommonEditorUtils::ChangePinCategoryToDouble(InputPin->PinType);
			break;
		case EFenixSelectorInputDataType::Prob:
			InputPin->PinName = PIN_NAME_PROBS;
			CommonEditorUtils::ChangePinCategoryToDouble(InputPin->PinType);
			break;
		case EFenixSelectorInputDataType::WeightOrProb:
			InputPin->PinName = PIN_NAME_WEIGHT_OR_PROBS;
			CommonEditorUtils::ChangePinCategoryToWeightOrProbEntry(InputPin->PinType);
			break;
		}
		break;
	case EFenixSelectorInputFormat::Map:
		switch (NewDataType)
		{
		case EFenixSelectorInputDataType::Weight:
			InputPin->PinName = PIN_NAME_WEIGHT_MAP;
			CommonEditorUtils::ChangePinValueCategoryToDouble(InputPin->PinType.PinValueType);
			break;
		case EFenixSelectorInputDataType::Prob:
			InputPin->PinName = PIN_NAME_PROB_MAP;
			CommonEditorUtils::ChangePinValueCategoryToDouble(InputPin->PinType.PinValueType);
			break;
		case EFenixSelectorInputDataType::WeightOrProb:
			InputPin->PinName = PIN_NAME_WEIGHT_OR_PROB_MAP;
			CommonEditorUtils::ChangePinValueCategoryToWeightOrProbEntry(InputPin->PinType.PinValueType);
			break;
		}
		break;
	case EFenixSelectorInputFormat::DataTable:
		switch (NewDataType)
		{
		case EFenixSelectorInputDataType::Weight:
			InputDataTableWeightOrProbNamePin->PinName = PIN_NAME_WEIGHT_PROPERTY_NAME;
			InputDataTableWeightOrProbNamePin->DefaultValue = WeightPropertyName;
			if (InputDataTableIsProbNamePin)
			{
				RemovePin(InputDataTableIsProbNamePin);
			}
			break;
		case EFenixSelectorInputDataType::Prob:
			InputDataTableWeightOrProbNamePin->PinName = PIN_NAME_PROB_PROPERTY_NAME;
			InputDataTableWeightOrProbNamePin->DefaultValue = ProbPropertyName;
			if (InputDataTableIsProbNamePin)
			{
				RemovePin(InputDataTableIsProbNamePin);
			}
			break;
		case EFenixSelectorInputDataType::WeightOrProb:
			InputDataTableWeightOrProbNamePin->PinName = PIN_NAME_WEIGHT_OR_PROB_PROPERTY_NAME;
			InputDataTableWeightOrProbNamePin->DefaultValue = WeightOrProbPropertyName;
			if (!InputDataTableIsProbNamePin)
			{
				InputDataTableIsProbNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_IS_PROB_PROPERTY_NAME);
				InputDataTableIsProbNamePin->DefaultValue = IsProbPropertyName;
			}
			break;
		}
		break;
	}

	// Output pins
	UEdGraphPin* OutputPin = GetOutputPin();
	switch (NewDataType)
	{
	case EFenixSelectorInputDataType::Weight:
		if (!OutputPin->SubPins.IsEmpty())
		{
			GetSchema()->RecombinePin(OutputPin->SubPins[0]);
		}
		OutputPin->PinName = PIN_NAME_CUM_WEIGHTS;
		CommonEditorUtils::ChangePinTypeToDoubleArray(OutputPin->PinType);
		break;
	case EFenixSelectorInputDataType::Prob:
		if (!OutputPin->SubPins.IsEmpty())
		{
			GetSchema()->RecombinePin(OutputPin->SubPins[0]);
		}
		OutputPin->PinName = PIN_NAME_CUM_PROBS;
		CommonEditorUtils::ChangePinTypeToDoubleArray(OutputPin->PinType);
		break;
	case EFenixSelectorInputDataType::WeightOrProb:
		OutputPin->PinName = PIN_NAME_COOKED_DISTRIBUTION;
		CommonEditorUtils::ChangePinTypeToCookedSelectorDistribution(OutputPin->PinType);
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

void UK2Node_CookSelectorInput::OnFormatPinUpdated(UEdGraphPin* ChangedPin)
{
	// Get new format
	UEnum* FormatTypeObject = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EFenixSelectorInputFormat"), true);
	const EFenixSelectorInputFormat NewFormat = static_cast<EFenixSelectorInputFormat>(FormatTypeObject->GetValueByNameString(ChangedPin->DefaultValue));

	// Input pins
	UEdGraphPin* InputPin = GetInputPin();
	UEdGraphPin* InputDataTableWeightOrProbNamePin = GetInputDataTableWeightOrProbNamePin();
	UEdGraphPin* InputDataTableIsProbNamePin = GetInputDataTableIsProbNamePin();
	switch (NewFormat)
	{
	case EFenixSelectorInputFormat::Array:
		switch (CurrentDataType)
		{
		case EFenixSelectorInputDataType::Weight:
			InputPin->PinName = PIN_NAME_WEIGHTS;
			CommonEditorUtils::ChangePinTypeToDoubleArray(InputPin->PinType);
			break;
		case EFenixSelectorInputDataType::Prob:
			InputPin->PinName = PIN_NAME_PROBS;
			CommonEditorUtils::ChangePinTypeToDoubleArray(InputPin->PinType);
			break;
		case EFenixSelectorInputDataType::WeightOrProb:
			InputPin->PinName = PIN_NAME_WEIGHT_OR_PROBS;
			CommonEditorUtils::ChangePinTypeToWeightOrProbArray(InputPin->PinType);
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
	case EFenixSelectorInputFormat::Map:
		switch (CurrentDataType)
		{
		case EFenixSelectorInputDataType::Weight:
			InputPin->PinName = PIN_NAME_WEIGHT_MAP;
			CommonEditorUtils::ChangePinTypeToDoubleMap(InputPin->PinType);
			break;
		case EFenixSelectorInputDataType::Prob:
			InputPin->PinName = PIN_NAME_PROB_MAP;
			CommonEditorUtils::ChangePinTypeToDoubleMap(InputPin->PinType);
			break;
		case EFenixSelectorInputDataType::WeightOrProb:
			InputPin->PinName = PIN_NAME_WEIGHT_OR_PROB_MAP;
			CommonEditorUtils::ChangePinTypeToWeightOrProbMap(InputPin->PinType);
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
	case EFenixSelectorInputFormat::DataTable:
		InputPin->PinName = PIN_NAME_DATA_TABLE;
		CommonEditorUtils::ChangePinTypeToDataTable(InputPin->PinType);
		InputPin->DefaultObject = DataTable;
		switch (CurrentDataType)
		{
		case EFenixSelectorInputDataType::Weight:
			if (!InputDataTableWeightOrProbNamePin)
			{
				InputDataTableWeightOrProbNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_WEIGHT_PROPERTY_NAME);
				InputDataTableWeightOrProbNamePin->DefaultValue = WeightPropertyName;
			}
			break;
		case EFenixSelectorInputDataType::Prob:
			if (!InputDataTableWeightOrProbNamePin)
			{
				InputDataTableWeightOrProbNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_PROB_PROPERTY_NAME);
				InputDataTableWeightOrProbNamePin->DefaultValue = ProbPropertyName;
			}
			break;
		case EFenixSelectorInputDataType::WeightOrProb:
			if (!InputDataTableWeightOrProbNamePin)
			{
				InputDataTableWeightOrProbNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_WEIGHT_OR_PROB_PROPERTY_NAME);
				InputDataTableWeightOrProbNamePin->DefaultValue = WeightOrProbPropertyName;
			}
			if (!InputDataTableIsProbNamePin)
			{
				InputDataTableIsProbNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_IS_PROB_PROPERTY_NAME);
				InputDataTableIsProbNamePin->DefaultValue = IsProbPropertyName;
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
	case EFenixSelectorInputFormat::Array:
		if (OutputKeysPin)
		{
			RemovePin(OutputKeysPin);
		}
		break;
	case EFenixSelectorInputFormat::Map:
		if (!OutputKeysPin)
		{
			CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, PIN_NAME_KEYS, OutKeysPinParams);
		}
		else
		{
			OutputKeysPin->PinName = PIN_NAME_KEYS;
			if (OutputKeysPin->LinkedTo.IsEmpty())
			{
				CommonEditorUtils::ChangePinCategoryToWildcard(OutputKeysPin->PinType);
			}
		}
		break;
	case EFenixSelectorInputFormat::DataTable:
		if (!OutputKeysPin)
		{
			CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Name, PIN_NAME_ROW_NAMES, OutKeysPinParams);
		}
		else
		{
			OutputKeysPin->PinName = PIN_NAME_ROW_NAMES;
			CommonEditorUtils::ChangePinCategoryToName(OutputKeysPin->PinType);
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
	if (CurrentFormat == EFenixSelectorInputFormat::Map)
	{
		ReconstructNode();
	}
}

void UK2Node_CookSelectorInput::OnInputPinUpdated(UEdGraphPin* ChangedPin)
{
	switch (CurrentFormat)
	{
	case EFenixSelectorInputFormat::DataTable:
		DataTable = ChangedPin->DefaultObject;
		break;
	}
}

void UK2Node_CookSelectorInput::OnDataTableWeightOrProbNamePinUpdated(UEdGraphPin* ChangedPin)
{
	switch (CurrentDataType)
	{
	case EFenixSelectorInputDataType::Weight:
		WeightPropertyName = ChangedPin->DefaultValue;
		break;
	case EFenixSelectorInputDataType::Prob:
		ProbPropertyName = ChangedPin->DefaultValue;
		break;
	case EFenixSelectorInputDataType::WeightOrProb:
		WeightOrProbPropertyName = ChangedPin->DefaultValue;
		break;
	}
}

void UK2Node_CookSelectorInput::OnDataTableIsProbNamePinUpdated(UEdGraphPin* ChangedPin)
{
	IsProbPropertyName = ChangedPin->DefaultValue;
}

FText UK2Node_CookSelectorInput::GetCurrentTooltip() const
{
	FText FormatString = FText::FromString("{0} {1}.");
	switch (CurrentDataType)
	{
	case EFenixSelectorInputDataType::Weight:
		FormatString = FText::FromString("Cook a weight {0} into cumulative weights{1}.");
		break;
	case EFenixSelectorInputDataType::Prob:
		FormatString = FText::FromString("Cook a probability {0} into cumulative probabilities{1}.");
		break;
	case EFenixSelectorInputDataType::WeightOrProb:
		FormatString = FText::FromString("Cook a \"weight or probability\" {0} into a CookedSelectorDistribution{1}.");
		break;
	}

	FText Arg0 = FText();
	FText Arg1 = FText();
	switch (CurrentFormat)
	{
	case EFenixSelectorInputFormat::Array:
		Arg0 = FText::FromString("array");
		break;
	case EFenixSelectorInputFormat::Map:
		Arg0 = FText::FromString("map");
		Arg1 = FText::FromString(" and keys");
		break;
	case EFenixSelectorInputFormat::DataTable:
		Arg0 = FText::FromString("data table");
		Arg1 = FText::FromString(" and row names");
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
	case EFenixSelectorInputFormat::Array:
		switch (CurrentDataType)
		{
		case EFenixSelectorInputDataType::Weight:
			return FindPin(PIN_NAME_WEIGHTS);
		case EFenixSelectorInputDataType::Prob:
			return FindPin(PIN_NAME_PROBS);
		case EFenixSelectorInputDataType::WeightOrProb:
			return FindPin(PIN_NAME_WEIGHT_OR_PROBS);
		}
		break;
	case EFenixSelectorInputFormat::Map:
		switch (CurrentDataType)
		{
		case EFenixSelectorInputDataType::Weight:
			return FindPin(PIN_NAME_WEIGHT_MAP);
		case EFenixSelectorInputDataType::Prob:
			return FindPin(PIN_NAME_PROB_MAP);
		case EFenixSelectorInputDataType::WeightOrProb:
			return FindPin(PIN_NAME_WEIGHT_OR_PROB_MAP);
		}
		break;
	case EFenixSelectorInputFormat::DataTable:
		return FindPin(PIN_NAME_DATA_TABLE);
	}
	return nullptr;
}

UEdGraphPin* UK2Node_CookSelectorInput::GetInputDataTableWeightOrProbNamePin()
{
	switch (CurrentDataType)
	{
	case EFenixSelectorInputDataType::Weight:
		return FindPin(PIN_NAME_WEIGHT_PROPERTY_NAME);
	case EFenixSelectorInputDataType::Prob:
		return FindPin(PIN_NAME_PROB_PROPERTY_NAME);
	case EFenixSelectorInputDataType::WeightOrProb:
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
	case EFenixSelectorInputDataType::Weight:
		return FindPin(PIN_NAME_CUM_WEIGHTS);
	case EFenixSelectorInputDataType::Prob:
		return FindPin(PIN_NAME_CUM_PROBS);
	case EFenixSelectorInputDataType::WeightOrProb:
		return FindPin(PIN_NAME_COOKED_DISTRIBUTION);
	}
	return nullptr;
}

UEdGraphPin* UK2Node_CookSelectorInput::GetOutputKeysPin()
{
	switch (CurrentFormat)
	{
	case EFenixSelectorInputFormat::Array:
		return nullptr;
	case EFenixSelectorInputFormat::Map:
		return FindPin(PIN_NAME_KEYS);
	case EFenixSelectorInputFormat::DataTable:
		return FindPin(PIN_NAME_ROW_NAMES);
	}
	return nullptr;
}
