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
#include "Kismet/KismetStringLibrary.h"

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

	FName FuncName;
	FName FuncInputPinName;

	switch (CurrentDataType)
	{
	case EFenixSelectorInputDataType::Weight:
		if (UseCookedInput)
		{
			FuncName = UseStream ? GET_FUNCTION_NAME_CHECKED(USelectorUtils, BPFunc_SelectWithCumWeightsFromStream) : GET_FUNCTION_NAME_CHECKED(USelectorUtils, BPFunc_SelectWithCumWeights);
			FuncInputPinName = "CumWeights";
		}
		else
		{
			FuncName = UseStream ? GET_FUNCTION_NAME_CHECKED(USelectorUtils, BPFunc_SelectWithWeightsFromStream) : GET_FUNCTION_NAME_CHECKED(USelectorUtils, BPFunc_SelectWithWeights);
			FuncInputPinName = "Weights";
		}
		break;
	case EFenixSelectorInputDataType::Prob:
		if (UseCookedInput)
		{
			FuncName = UseStream ? GET_FUNCTION_NAME_CHECKED(USelectorUtils, BPFunc_SelectWithCumProbsFromStream) : GET_FUNCTION_NAME_CHECKED(USelectorUtils, BPFunc_SelectWithCumProbs);
			FuncInputPinName = "CumProbs";
		}
		else
		{
			FuncName = UseStream ? GET_FUNCTION_NAME_CHECKED(USelectorUtils, BPFunc_SelectWithProbsFromStream) : GET_FUNCTION_NAME_CHECKED(USelectorUtils, BPFunc_SelectWithProbs);
			FuncInputPinName = "Probs";
		}
		break;
	case EFenixSelectorInputDataType::WeightOrProb:
		if (UseCookedInput)
		{
			FuncName = UseStream ? GET_FUNCTION_NAME_CHECKED(USelectorUtils, BPFunc_SelectWithCookedDistributionFromStream) : GET_FUNCTION_NAME_CHECKED(USelectorUtils, BPFunc_SelectWithCookedDistribution);
			FuncInputPinName = "Distribution";
		}
		else
		{
			FuncName = UseStream ? GET_FUNCTION_NAME_CHECKED(USelectorUtils, BPFunc_SelectWithWeightOrProbEntriesFromStream) : GET_FUNCTION_NAME_CHECKED(USelectorUtils, BPFunc_SelectWithWeightOrProbEntries);
			FuncInputPinName = "Entries";
		}
		break;
	}

	UK2Node_CallFunction* SelectFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	SelectFuncNode->FunctionReference.SetExternalMember(FuncName, USelectorUtils::StaticClass());
	SelectFuncNode->AllocateDefaultPins();

	UEdGraphPin* ExecPin = GetExecPin();
	UEdGraphPin* InputPin = GetInputPin();
	UEdGraphPin* OutputPin = GetOutputPin();
	UEdGraphPin* ThenPin = GetThenPin();
	UEdGraphPin* SelectFuncExecPin = SelectFuncNode->GetExecPin();
	UEdGraphPin* SelectFuncInputPin = SelectFuncNode->FindPin(FuncInputPinName);
	UEdGraphPin* SelectFuncOutputPin = SelectFuncNode->GetReturnValuePin();
	UEdGraphPin* SelectFuncThenPin = SelectFuncNode->GetThenPin();

	switch (CurrentFormat)
	{
	case EFenixSelectorInputFormat::Array: // this include the case where UseCookedInput is true
		{
			CompilerContext.MovePinLinksToIntermediate(*ExecPin, *SelectFuncExecPin);
			CompilerContext.MovePinLinksToIntermediate(*InputPin, *SelectFuncInputPin);
			CompilerContext.MovePinLinksToIntermediate(*OutputPin, *SelectFuncOutputPin);
			CompilerContext.MovePinLinksToIntermediate(*ThenPin, *SelectFuncThenPin);
		}
		break;
	case EFenixSelectorInputFormat::Map:
		{
			UEdGraphPin* OutputKeyPin = GetOutputKeyPin();
			const FEdGraphPinType& OutputKeyPinType = OutputKeyPin->PinType;

			FuncName = GET_FUNCTION_NAME_CHECKED(UBlueprintMapLibrary, Map_Keys);
			UK2Node_CallFunction* GetKeysFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
			GetKeysFuncNode->FunctionReference.SetExternalMember(FuncName, UBlueprintMapLibrary::StaticClass());
			GetKeysFuncNode->AllocateDefaultPins();
			UEdGraphPin* GetKeysInputPin = GetKeysFuncNode->FindPin(TEXT("TargetMap"));
			CommonEditorUtils::CopyPinTypeAndValueTypeInfo(GetKeysInputPin->PinType, InputPin->PinType);
			UEdGraphPin* GetKeysOutputPin = GetKeysFuncNode->FindPin(TEXT("Keys"));
			CommonEditorUtils::CopyPinTypeCategoryInfo(GetKeysOutputPin->PinType, OutputKeyPinType);

			FuncName = GET_FUNCTION_NAME_CHECKED(UBlueprintMapLibrary, Map_Values);
			UK2Node_CallFunction* GetValuesFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
			GetValuesFuncNode->FunctionReference.SetExternalMember(FuncName, UBlueprintMapLibrary::StaticClass());
			GetValuesFuncNode->AllocateDefaultPins();
			UEdGraphPin* GetValuesInputPin = GetValuesFuncNode->FindPin(TEXT("TargetMap"));
			CommonEditorUtils::CopyPinTypeAndValueTypeInfo(GetValuesInputPin->PinType, InputPin->PinType);
			UEdGraphPin* GetValuesOutputPin = GetValuesFuncNode->FindPin(TEXT("Values"));
			CommonEditorUtils::CopyPinTypeCategoryInfo(GetValuesOutputPin->PinType, SelectFuncInputPin->PinType);

			FuncName = GET_FUNCTION_NAME_CHECKED(UCommonUtils, Array_Get_Impure);
			UK2Node_CallFunction* GetItemFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
			GetItemFuncNode->FunctionReference.SetExternalMember(FuncName, UCommonUtils::StaticClass());
			GetItemFuncNode->AllocateDefaultPins();
			UEdGraphPin* GetItemInputArrayPin = GetItemFuncNode->FindPin(TEXT("TargetArray"));
			CommonEditorUtils::CopyPinTypeCategoryInfo(GetItemInputArrayPin->PinType, OutputKeyPinType);
			UEdGraphPin* GetItemInputIndexPin = GetItemFuncNode->FindPin(TEXT("Index"));
			UEdGraphPin* GetItemOutputItemPin = GetItemFuncNode->FindPin(TEXT("Item"));
			CommonEditorUtils::CopyPinTypeCategoryInfo(GetItemOutputItemPin->PinType, OutputKeyPinType);

			// Keys = GetKeys(Map) -> Values = GetValues(Map) -> SelectedIndex = Select(Values) => SelectedKey = GetItem(Keys, SelectedIndex) => Return (SelectedIndex, SelectedKey)
			CompilerContext.MovePinLinksToIntermediate(*ExecPin, *(GetKeysFuncNode->GetExecPin()));
			CompilerContext.CopyPinLinksToIntermediate(*InputPin, *GetKeysInputPin);
			GetKeysOutputPin->MakeLinkTo(GetItemInputArrayPin);
			GetKeysFuncNode->GetThenPin()->MakeLinkTo(GetValuesFuncNode->GetExecPin());
			CompilerContext.MovePinLinksToIntermediate(*InputPin, *GetValuesInputPin);
			GetValuesOutputPin->MakeLinkTo(SelectFuncInputPin);
			GetValuesFuncNode->GetThenPin()->MakeLinkTo(SelectFuncExecPin);
			CompilerContext.MovePinLinksToIntermediate(*OutputPin, *SelectFuncOutputPin);
			SelectFuncOutputPin->MakeLinkTo(GetItemInputIndexPin);
			SelectFuncThenPin->MakeLinkTo(GetItemFuncNode->GetExecPin());
			CompilerContext.MovePinLinksToIntermediate(*OutputKeyPin, *GetItemOutputItemPin);
			CompilerContext.MovePinLinksToIntermediate(*ThenPin, *GetItemFuncNode->GetThenPin());
		}
		break;
	case EFenixSelectorInputFormat::DataTable:
		{
			UEdGraphPin* OutputKeyPin = GetOutputKeyPin();
			const FEdGraphPinType& OutputKeyPinType = OutputKeyPin->PinType;

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

			FuncName = GET_FUNCTION_NAME_CHECKED(UCommonUtils, Array_Get_Impure);
			UK2Node_CallFunction* GetItemFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
			GetItemFuncNode->FunctionReference.SetExternalMember(FuncName, UCommonUtils::StaticClass());
			GetItemFuncNode->AllocateDefaultPins();
			UEdGraphPin* GetItemInputArrayPin = GetItemFuncNode->FindPin(TEXT("TargetArray"));
			CommonEditorUtils::CopyPinTypeCategoryInfo(GetItemInputArrayPin->PinType, OutputKeyPinType);
			UEdGraphPin* GetItemInputIndexPin = GetItemFuncNode->FindPin(TEXT("Index"));
			UEdGraphPin* GetItemOutputItemPin = GetItemFuncNode->FindPin(TEXT("Item"));
			CommonEditorUtils::CopyPinTypeCategoryInfo(GetItemOutputItemPin->PinType, OutputKeyPinType);

			// Keys = GetRowNames(DataTable) -> Values = GetValues(DataTable, LabelNames) -> SelectedIndex = Select(Values) => SelectedKey = GetItem(Keys, SelectedIndex) => Return (SelectedIndex, SelectedKey)
			CompilerContext.MovePinLinksToIntermediate(*ExecPin, *(GetKeysFuncNode->GetExecPin()));
			CompilerContext.CopyPinLinksToIntermediate(*InputPin, *GetKeysInputPin);
			GetKeysOutputPin->MakeLinkTo(GetItemInputArrayPin);
			GetKeysFuncNode->GetThenPin()->MakeLinkTo(GetValuesFuncNode->GetExecPin());
			CompilerContext.MovePinLinksToIntermediate(*InputPin, *GetValuesInputPin);
			CompilerContext.MovePinLinksToIntermediate(*GetInputDataTableWeightOrProbNamePin(), *GetValuesWeightOrProbNamePin);
			if (GetValuesIsProbNamePin)
			{
				CompilerContext.MovePinLinksToIntermediate(*GetInputDataTableIsProbNamePin(), *GetValuesIsProbNamePin);
			}
			GetValuesOutputPin->MakeLinkTo(SelectFuncInputPin);
			GetValuesFuncNode->GetThenPin()->MakeLinkTo(SelectFuncExecPin);
			CompilerContext.MovePinLinksToIntermediate(*OutputPin, *SelectFuncOutputPin);
			SelectFuncOutputPin->MakeLinkTo(GetItemInputIndexPin);
			SelectFuncThenPin->MakeLinkTo(GetItemFuncNode->GetExecPin());
			CompilerContext.MovePinLinksToIntermediate(*OutputKeyPin, *GetItemOutputItemPin);
			CompilerContext.MovePinLinksToIntermediate(*ThenPin, *GetItemFuncNode->GetThenPin());
		}
		break;
	}

	if (UseStream)
	{
		UEdGraphPin* StreamPin = GetStreamPin();
		UEdGraphPin* SelectFuncStreamPin = SelectFuncNode->FindPin(TEXT("RandomStream"));
		CompilerContext.MovePinLinksToIntermediate(*StreamPin, *SelectFuncStreamPin);
	}

	BreakAllNodeLinks();
}

void UK2Node_RandomSelect::AllocateDefaultPins()
{
	// Add execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);

	// Add data type pin
	static UEnum* DataTypeTypeObject = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EFenixSelectorInputDataType"), true);
	UEdGraphPin* DataTypePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, DataTypeTypeObject, PIN_NAME_DATA_TYPE);
	DataTypePin->DefaultValue = DataTypeTypeObject->GetNameStringByValue(static_cast<int64>(CurrentDataType));
	DataTypePin->bNotConnectable = true;

	// Add format pin (only if not using cooked input)
	if (UseCookedInput)
	{
		CurrentFormat = EFenixSelectorInputFormat::Array;
	}
	else
	{
		static UEnum* FormatTypeObject = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EFenixSelectorInputFormat"), true);
		UEdGraphPin* FormatPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, FormatTypeObject, PIN_NAME_FORMAT);
		FormatPin->DefaultValue = FormatTypeObject->GetNameStringByValue(static_cast<int64>(CurrentFormat));
		FormatPin->bNotConnectable = true;
	}

	// Add use cooked input pin
	UEdGraphPin* UseCookedInputPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, PIN_NAME_USE_COOKED_INPUT);
	UseCookedInputPin->DefaultValue = UKismetStringLibrary::Conv_BoolToString(UseCookedInput);
	UseCookedInputPin->bNotConnectable = true;

	// Add use stream pin
	UEdGraphPin* UseStreamPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, PIN_NAME_USE_STREAM);
	UseStreamPin->DefaultValue = UKismetStringLibrary::Conv_BoolToString(UseStream);
	UseStreamPin->bNotConnectable = true;

	// Add other pins
	CreateInOutPins();

	Super::AllocateDefaultPins();
}

void UK2Node_RandomSelect::PinDefaultValueChanged(UEdGraphPin* ChangedPin)
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
	else if (ChangedPin == GetUseCookedInputPin())
	{
		OnUseCookedInputPinUpdated(ChangedPin);
	}
	else if (ChangedPin == GetUseStreamPin())
	{
		OnUseStreamPinUpdated(ChangedPin);
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

void UK2Node_RandomSelect::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::PinConnectionListChanged(Pin);

	if (CurrentFormat == EFenixSelectorInputFormat::Map)
	{
		UEdGraphPin* InputPin = GetInputPin();
		UEdGraphPin* OutputKeyPin = GetOutputKeyPin();
		if (Pin == InputPin)
		{
			CommonEditorUtils::OnPinConnectionUpdatedWithCategoryInfoSync(InputPin, OutputKeyPin);
		}
		else if (Pin == OutputKeyPin)
		{
			CommonEditorUtils::OnPinConnectionUpdatedWithCategoryInfoSync(OutputKeyPin, InputPin);
		}
	}
}

void UK2Node_RandomSelect::PostReconstructNode()
{
	Super::PostReconstructNode();

	// Redo wildcard type-matching after reconstruction
	if (CurrentFormat == EFenixSelectorInputFormat::Map)
	{
		UEdGraphPin* InputPin = GetInputPin();
		UEdGraphPin* OutputKeyPin = GetOutputKeyPin();
		const bool updated = CommonEditorUtils::PostPinConnectionReconstructionWithCategoryInfoSync(InputPin, OutputKeyPin);
		if (!updated)
		{
			CommonEditorUtils::PostPinConnectionReconstructionWithCategoryInfoSync(OutputKeyPin, InputPin);
		}
	}
}

void UK2Node_RandomSelect::CreateInOutPins()
{
	// Input pins (not including stream)
	FCreatePinParams InPinParams;
	UEdGraphPin* InputPin;
	if (UseCookedInput)
	{
		InPinParams.bIsReference = true;
		InPinParams.bIsConst = true;
		switch (CurrentDataType)
		{
		case EFenixSelectorInputDataType::Weight:
			InPinParams.ContainerType = EPinContainerType::Array;
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Real, UEdGraphSchema_K2::PC_Double, PIN_NAME_CUM_WEIGHTS, InPinParams);
			break;
		case EFenixSelectorInputDataType::Prob:
			InPinParams.ContainerType = EPinContainerType::Array;
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Real, UEdGraphSchema_K2::PC_Double, PIN_NAME_CUM_PROBS, InPinParams);
			break;
		case EFenixSelectorInputDataType::WeightOrProb:
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FCookedSelectorDistribution::StaticStruct(), PIN_NAME_COOKED_DISTRIBUTION, InPinParams);
			break;
		}
	}
	else
	{
		UEdGraphPin* InputDataTableWeightOrProbNamePin;
		UEdGraphPin* InputDataTableIsProbNamePin;
		switch (CurrentFormat)
		{
		case EFenixSelectorInputFormat::Array:
			InPinParams.ContainerType = EPinContainerType::Array;
			InPinParams.bIsReference = true;
			InPinParams.bIsConst = true;
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
			InPinParams.bIsReference = true;
			InPinParams.bIsConst = true;
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
			InPinParams.bIsConst = true;
			InputPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UDataTable::StaticClass(), PIN_NAME_DATA_TABLE, InPinParams);
			InputPin->DefaultObject = DataTable;
			switch (CurrentDataType)
			{
			case EFenixSelectorInputDataType::Weight:
				InputDataTableWeightOrProbNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_WEIGHT_PROPERTY_NAME, InPinParams);
				InputDataTableWeightOrProbNamePin->DefaultValue = WeightPropertyName;
				break;
			case EFenixSelectorInputDataType::Prob:
				InputDataTableWeightOrProbNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_PROB_PROPERTY_NAME, InPinParams);
				InputDataTableWeightOrProbNamePin->DefaultValue = ProbPropertyName;
				break;
			case EFenixSelectorInputDataType::WeightOrProb:
				InputDataTableWeightOrProbNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_WEIGHT_OR_PROB_PROPERTY_NAME, InPinParams);
				InputDataTableWeightOrProbNamePin->DefaultValue = WeightOrProbPropertyName;
				InputDataTableIsProbNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_IS_PROB_PROPERTY_NAME, InPinParams);
				InputDataTableIsProbNamePin->DefaultValue = IsProbPropertyName;
				break;
			}
			break;
		}
	}

	// Input stream pin
	if (UseStream)
	{
		FCreatePinParams StreamPinParams;
		StreamPinParams.bIsReference = true;
		StreamPinParams.bIsConst = true;
		static UScriptStruct* StreamTypeObject = TBaseStructure<FRandomStream>::Get();
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, StreamTypeObject, PIN_NAME_RANDOM_STREAM, StreamPinParams);
	}


	// Output Pins
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Int, PIN_NAME_SELECTED_INEX);
	switch (CurrentFormat)
	{
	case EFenixSelectorInputFormat::Map:
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, PIN_NAME_SELECTED_KEY);
		break;
	case EFenixSelectorInputFormat::DataTable:
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Name, PIN_NAME_SELECTED_ROW_NAME);
		break;
	}
}

void UK2Node_RandomSelect::OnDataTypePinUpdated(UEdGraphPin* ChangedPin)
{
	// Get new data type
	static UEnum* DataTypeTypeObject = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EFenixSelectorInputDataType"), true);
	const EFenixSelectorInputDataType NewDataType = static_cast<EFenixSelectorInputDataType>(DataTypeTypeObject->GetValueByNameString(ChangedPin->DefaultValue));

	// Input pins
	FCreatePinParams InPinParams;
	UEdGraphPin* InputPin = GetInputPin();
	UEdGraphPin* InputDataTableWeightOrProbNamePin = GetInputDataTableWeightOrProbNamePin();
	UEdGraphPin* InputDataTableIsProbNamePin = GetInputDataTableIsProbNamePin();
	if (UseCookedInput)
	{
		switch (NewDataType)
		{
		case EFenixSelectorInputDataType::Weight:
			if (!InputPin->SubPins.IsEmpty())
			{
				GetSchema()->RecombinePin(InputPin->SubPins[0]);
			}
			InputPin->PinName = PIN_NAME_CUM_WEIGHTS;
			CommonEditorUtils::ChangePinTypeToDoubleArrayRef(InputPin->PinType);
			break;
		case EFenixSelectorInputDataType::Prob:
			if (!InputPin->SubPins.IsEmpty())
			{
				GetSchema()->RecombinePin(InputPin->SubPins[0]);
			}
			InputPin->PinName = PIN_NAME_CUM_PROBS;
			CommonEditorUtils::ChangePinTypeToDoubleArrayRef(InputPin->PinType);
			break;
		case EFenixSelectorInputDataType::WeightOrProb:
			InputPin->PinName = PIN_NAME_COOKED_DISTRIBUTION;
			CommonEditorUtils::ChangePinTypeToCookedSelectorDistributionRef(InputPin->PinType);
			break;
		}
	}
	else
	{
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
					InPinParams.bIsConst = true;
					InPinParams.Index = 8;
					InputDataTableIsProbNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_IS_PROB_PROPERTY_NAME, InPinParams);
					InputDataTableIsProbNamePin->DefaultValue = IsProbPropertyName;
				}
				break;
			}
			break;
		}
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
}

void UK2Node_RandomSelect::OnFormatPinUpdated(UEdGraphPin* ChangedPin)
{
	// Get new format
	static UEnum* FormatTypeObject = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EFenixSelectorInputFormat"), true);
	const EFenixSelectorInputFormat NewFormat = static_cast<EFenixSelectorInputFormat>(FormatTypeObject->GetValueByNameString(ChangedPin->DefaultValue));

	// Input pins
	FCreatePinParams InPinParams;
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
			CommonEditorUtils::ChangePinTypeToDoubleArrayRef(InputPin->PinType);
			break;
		case EFenixSelectorInputDataType::Prob:
			InputPin->PinName = PIN_NAME_PROBS;
			CommonEditorUtils::ChangePinTypeToDoubleArrayRef(InputPin->PinType);
			break;
		case EFenixSelectorInputDataType::WeightOrProb:
			InputPin->PinName = PIN_NAME_WEIGHT_OR_PROBS;
			CommonEditorUtils::ChangePinTypeToWeightOrProbArrayRef(InputPin->PinType);
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
			CommonEditorUtils::ChangePinTypeToDoubleMapRef(InputPin->PinType);
			break;
		case EFenixSelectorInputDataType::Prob:
			InputPin->PinName = PIN_NAME_PROB_MAP;
			CommonEditorUtils::ChangePinTypeToDoubleMapRef(InputPin->PinType);
			break;
		case EFenixSelectorInputDataType::WeightOrProb:
			InputPin->PinName = PIN_NAME_WEIGHT_OR_PROB_MAP;
			CommonEditorUtils::ChangePinTypeToWeightOrProbMapRef(InputPin->PinType);
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
		InPinParams.bIsConst = true;
		InputPin->PinName = PIN_NAME_DATA_TABLE;
		CommonEditorUtils::ChangePinTypeToDataTablePtr(InputPin->PinType);
		InputPin->DefaultObject = DataTable;
		switch (CurrentDataType)
		{
		case EFenixSelectorInputDataType::Weight:
			if (!InputDataTableWeightOrProbNamePin)
			{
				InPinParams.Index = 7;
				InputDataTableWeightOrProbNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_WEIGHT_PROPERTY_NAME, InPinParams);
				InputDataTableWeightOrProbNamePin->DefaultValue = WeightPropertyName;
			}
			break;
		case EFenixSelectorInputDataType::Prob:
			if (!InputDataTableWeightOrProbNamePin)
			{
				InPinParams.Index = 7;
				InputDataTableWeightOrProbNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_PROB_PROPERTY_NAME, InPinParams);
				InputDataTableWeightOrProbNamePin->DefaultValue = ProbPropertyName;
			}
			break;
		case EFenixSelectorInputDataType::WeightOrProb:
			if (!InputDataTableWeightOrProbNamePin)
			{
				InPinParams.Index = 7;
				InputDataTableWeightOrProbNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_WEIGHT_OR_PROB_PROPERTY_NAME, InPinParams);
				InputDataTableWeightOrProbNamePin->DefaultValue = WeightOrProbPropertyName;
			}
			if (!InputDataTableIsProbNamePin)
			{
				InPinParams.Index = 8;
				InputDataTableIsProbNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, PIN_NAME_IS_PROB_PROPERTY_NAME, InPinParams);
				InputDataTableIsProbNamePin->DefaultValue = IsProbPropertyName;
			}
			break;
		}
		break;
	}

	// Output pins
	UEdGraphPin* OutputKeyPin = GetOutputKeyPin();
	switch (NewFormat)
	{
	case EFenixSelectorInputFormat::Array:
		if (OutputKeyPin)
		{
			if (!OutputKeyPin->SubPins.IsEmpty())
			{
				GetSchema()->RecombinePin(OutputKeyPin->SubPins[0]);
			}
			RemovePin(OutputKeyPin);
		}
		break;
	case EFenixSelectorInputFormat::Map:
		if (!OutputKeyPin)
		{
			CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, PIN_NAME_SELECTED_KEY);
		}
		else
		{
			OutputKeyPin->PinName = PIN_NAME_SELECTED_KEY;
			if (OutputKeyPin->LinkedTo.IsEmpty())
			{
				CommonEditorUtils::ChangePinCategoryToWildcard(OutputKeyPin->PinType);
			}
		}
		break;
	case EFenixSelectorInputFormat::DataTable:
		if (!OutputKeyPin)
		{
			CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Name, PIN_NAME_SELECTED_ROW_NAME);
		}
		else
		{
			OutputKeyPin->PinName = PIN_NAME_SELECTED_ROW_NAME;
			CommonEditorUtils::ChangePinCategoryToName(OutputKeyPin->PinType);
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
	UEdGraphPin* NewOutputKeysPin = GetOutputKeyPin();
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

void UK2Node_RandomSelect::OnUseCookedInputPinUpdated(UEdGraphPin* ChangedPin)
{
	// Get new use cooked input flag
	const bool NewUseCookedInput = ChangedPin->DefaultValue.ToBool();

	// Format Pin and InputPin (not including data table property names pin and stream pin)
	UEdGraphPin* FormatPin = GetFormatPin();
	UEdGraphPin* InputPin = GetInputPin();
	if (NewUseCookedInput)
	{
		if (FormatPin)
		{
			RemovePin(FormatPin);
		}
		
		switch (CurrentDataType)
		{
		case EFenixSelectorInputDataType::Weight:
			InputPin->PinName = PIN_NAME_CUM_WEIGHTS;
			CommonEditorUtils::ChangePinTypeToDoubleArrayRef(InputPin->PinType);
			break;
		case EFenixSelectorInputDataType::Prob:
			InputPin->PinName = PIN_NAME_CUM_PROBS;
			CommonEditorUtils::ChangePinTypeToDoubleArrayRef(InputPin->PinType);
			break;
		case EFenixSelectorInputDataType::WeightOrProb:
			InputPin->PinName = PIN_NAME_COOKED_DISTRIBUTION;
			CommonEditorUtils::ChangePinTypeToCookedSelectorDistributionRef(InputPin->PinType);
			break;
		}
	}
	else
	{
		if (!FormatPin)
		{
			FCreatePinParams FormatPinParams;
			FormatPinParams.Index = 3;
			static UEnum* FormatTypeObject = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EFenixSelectorInputFormat"), true);
			FormatPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, FormatTypeObject, PIN_NAME_FORMAT, FormatPinParams);
			FormatPin->DefaultValue = FormatTypeObject->GetNameStringByValue(static_cast<int64>(EFenixSelectorInputFormat::Array));
			FormatPin->bNotConnectable = true;
		}

		if (!InputPin->SubPins.IsEmpty())
		{
			GetSchema()->RecombinePin(InputPin->SubPins[0]);
		}
		switch (CurrentDataType)
		{
		case EFenixSelectorInputDataType::Weight:
			InputPin->PinName = PIN_NAME_WEIGHTS;
			CommonEditorUtils::ChangePinTypeToDoubleArrayRef(InputPin->PinType);
			break;
		case EFenixSelectorInputDataType::Prob:
			InputPin->PinName = PIN_NAME_PROBS;
			CommonEditorUtils::ChangePinTypeToDoubleArrayRef(InputPin->PinType);
			break;
		case EFenixSelectorInputDataType::WeightOrProb:
			InputPin->PinName = PIN_NAME_WEIGHT_OR_PROBS;
			CommonEditorUtils::ChangePinTypeToWeightOrProbArrayRef(InputPin->PinType);
			break;
		}
	}

	// Remove pins that cannot apply to array format
	UEdGraphPin* InputDataTableWeightOrProbNamePin = GetInputDataTableWeightOrProbNamePin();
	if (InputDataTableWeightOrProbNamePin)
	{
		RemovePin(InputDataTableWeightOrProbNamePin);
	}
	UEdGraphPin* InputDataTableIsProbNamePin = GetInputDataTableIsProbNamePin();
	if (InputDataTableIsProbNamePin)
	{
		RemovePin(InputDataTableIsProbNamePin);
	}
	UEdGraphPin* OutputKeyPin = GetOutputKeyPin();
	if (OutputKeyPin)
	{
		if (!OutputKeyPin->SubPins.IsEmpty())
		{
			GetSchema()->RecombinePin(OutputKeyPin->SubPins[0]);
		}
		RemovePin(OutputKeyPin);
	}

	// Update the use cooked input flag and set format to be array
	UseCookedInput = NewUseCookedInput;
	CurrentFormat = EFenixSelectorInputFormat::Array;

	// Mark dirty/modified
	CachedToolTip.MarkDirty();
	FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());
	GetGraph()->NotifyGraphChanged();

	// Refresh type related neighbor nodes
	if (!InputPin->LinkedTo.IsEmpty())
	{
		InputPin->LinkedTo[0]->GetOwningNode()->ReconstructNode();
	}
}

void UK2Node_RandomSelect::OnUseStreamPinUpdated(UEdGraphPin* ChangedPin)
{
	// Get new use stream flag
	UseStream = ChangedPin->DefaultValue.ToBool();

	// Create/remove stream pin
	UEdGraphPin* StreamPin = GetStreamPin();
	if (UseStream)
	{
		if (!StreamPin)
		{
			FCreatePinParams StreamPinParams;
			StreamPinParams.bIsReference = true;
			StreamPinParams.bIsConst = true;
			static UScriptStruct* StreamTypeObject = TBaseStructure<FRandomStream>::Get();
			CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, StreamTypeObject, PIN_NAME_RANDOM_STREAM, StreamPinParams);
		}
	}
	else
	{
		if (StreamPin)
		{
			if (!StreamPin->SubPins.IsEmpty())
			{
				GetSchema()->RecombinePin(StreamPin->SubPins[0]);
			}
			RemovePin(StreamPin);
		}
	}

	// Mark dirty/modified
	CachedToolTip.MarkDirty();
	FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());
	GetGraph()->NotifyGraphChanged();
}

void UK2Node_RandomSelect::OnInputPinUpdated(UEdGraphPin* ChangedPin)
{
	switch (CurrentFormat)
	{
	case EFenixSelectorInputFormat::DataTable:
		DataTable = ChangedPin->DefaultObject;
		break;
	}
}

void UK2Node_RandomSelect::OnDataTableWeightOrProbNamePinUpdated(UEdGraphPin* ChangedPin)
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

void UK2Node_RandomSelect::OnDataTableIsProbNamePinUpdated(UEdGraphPin* ChangedPin)
{
	IsProbPropertyName = ChangedPin->DefaultValue;
}

UEdGraphPin* UK2Node_RandomSelect::GetDataTypePin()
{
	return FindPin(PIN_NAME_DATA_TYPE);
}

UEdGraphPin* UK2Node_RandomSelect::GetFormatPin()
{
	return FindPin(PIN_NAME_FORMAT);
}

UEdGraphPin* UK2Node_RandomSelect::GetUseCookedInputPin()
{
	return FindPin(PIN_NAME_USE_COOKED_INPUT);
}

UEdGraphPin* UK2Node_RandomSelect::GetUseStreamPin()
{
	return FindPin(PIN_NAME_USE_STREAM);
}

UEdGraphPin* UK2Node_RandomSelect::GetInputPin()
{
	if (UseCookedInput)
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

UEdGraphPin* UK2Node_RandomSelect::GetInputDataTableWeightOrProbNamePin()
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

UEdGraphPin* UK2Node_RandomSelect::GetInputDataTableIsProbNamePin()
{
	return FindPin(PIN_NAME_IS_PROB_PROPERTY_NAME);
}

UEdGraphPin* UK2Node_RandomSelect::GetStreamPin()
{
	return FindPin(PIN_NAME_RANDOM_STREAM);
}

UEdGraphPin* UK2Node_RandomSelect::GetOutputPin()
{
	return FindPin(PIN_NAME_SELECTED_INEX);
}

UEdGraphPin* UK2Node_RandomSelect::GetOutputKeyPin()
{
	switch (CurrentFormat)
	{
	case EFenixSelectorInputFormat::Map:
		return FindPin(PIN_NAME_SELECTED_KEY);
	case EFenixSelectorInputFormat::DataTable:
		return FindPin(PIN_NAME_SELECTED_ROW_NAME);
	}
	return nullptr;
}
