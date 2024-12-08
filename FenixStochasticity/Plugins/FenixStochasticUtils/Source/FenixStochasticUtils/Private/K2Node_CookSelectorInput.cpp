// Copyright 2024, Tiannan Chen, All rights reserved.


#include "K2Node_CookSelectorInput.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "Kismet2/BlueprintEditorUtils.h"

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

	FName FunctionName;
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
	CompilerContext.MovePinLinksToIntermediate(*(GetOutputPin()), *(CallFuncNode->GetReturnValuePin()));
	
	BreakAllNodeLinks();
}

void UK2Node_CookSelectorInput::PinDefaultValueChanged(UEdGraphPin* ChangedPin)
{
	if (ChangedPin == GetInputFormatPin())
	{
		OnInputFormatUpdated();
	}
}

void UK2Node_CookSelectorInput::AllocateDefaultPins()
{
	UEnum* InputFormatTypeObject = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EFenixCookSelectorInputFormat"), true);
	UEdGraphPin* InputFormatPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, InputFormatTypeObject, TEXT("InputFormat"));
	InputFormatPin->DefaultValue = InputFormatTypeObject->GetNameStringByValue(static_cast<int64>(CurrentInputFormat));

	CreateInOutPins();

	Super::AllocateDefaultPins();
}

FText UK2Node_CookSelectorInput::GetCurrentTooltip() const
{
	switch(CurrentInputFormat)
	{
	case EFenixCookSelectorInputFormat::Basic:
		return FText::FromString("Cook Selector Input, Basic Version");
	case EFenixCookSelectorInputFormat::Array:
		return FText::FromString("Cook Selector Input, Array Version");
	case EFenixCookSelectorInputFormat::Set:
		return FText::FromString("Cook Selector Input, Set Version");
	case EFenixCookSelectorInputFormat::Map:
		return FText::FromString("Cook Selector Input, Map Version");
	}

	return FText();
}

void UK2Node_CookSelectorInput::OnInputFormatUpdated()
{
	RemoveInOutPins();
	UEnum* InputFormatTypeObject = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EFenixCookSelectorInputFormat"), true);
	CurrentInputFormat = static_cast<EFenixCookSelectorInputFormat>(InputFormatTypeObject->GetValueByNameString(GetInputFormatPin()->DefaultValue));
	CreateInOutPins();
	CachedToolTip.MarkDirty();
	GetGraph()->NotifyGraphChanged();
	FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());
}

void UK2Node_CookSelectorInput::RemoveInOutPins()
{
	UEdGraphPin* InputPin = GetInputPin();
	if (InputPin)
	{
		RemovePin(InputPin);
	}

	UEdGraphPin* OutputPin = GetOutputPin();
	if (OutputPin)
	{
		RemovePin(OutputPin);
	}
}

void UK2Node_CookSelectorInput::CreateInOutPins()
{
	FCreatePinParams PinParams;
	switch (CurrentInputFormat)
	{
	case EFenixCookSelectorInputFormat::Basic:
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Int, TEXT("RawInt"));
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Int, TEXT("CookedInt"));
		break;
	case EFenixCookSelectorInputFormat::Array:
		PinParams.ContainerType = EPinContainerType::Array;
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Int, TEXT("RawArray"), PinParams);
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Int, TEXT("CookedArray"), PinParams);
		break;
	case EFenixCookSelectorInputFormat::Set:
		PinParams.ContainerType = EPinContainerType::Set;
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Int, TEXT("RawSet"), PinParams);
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Int, TEXT("CookedSet"), PinParams);
		break;
	case EFenixCookSelectorInputFormat::Map:
		PinParams.ContainerType = EPinContainerType::Map;
		PinParams.ValueTerminalType.TerminalCategory = UEdGraphSchema_K2::PC_String;
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Int, TEXT("RawMap"), PinParams);
		CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Int, TEXT("CookedMap"), PinParams);
		break;
	}
}

UEdGraphPin* UK2Node_CookSelectorInput::GetInputFormatPin()
{
	return FindPin(TEXT("InputFormat"));
}

UEdGraphPin* UK2Node_CookSelectorInput::GetInputPin()
{
	switch (CurrentInputFormat)
	{
	case EFenixCookSelectorInputFormat::Basic:
		return FindPin(TEXT("RawInt"));
	case EFenixCookSelectorInputFormat::Array:
		return FindPin(TEXT("RawArray"));
	case EFenixCookSelectorInputFormat::Set:
		return FindPin(TEXT("RawSet"));
	case EFenixCookSelectorInputFormat::Map:
		return FindPin(TEXT("RawMap"));
	}
	return nullptr;
}

UEdGraphPin* UK2Node_CookSelectorInput::GetOutputPin()
{
	switch (CurrentInputFormat)
	{
	case EFenixCookSelectorInputFormat::Basic:
		return FindPin(TEXT("CookedInt"));
	case EFenixCookSelectorInputFormat::Array:
		return FindPin(TEXT("CookedArray"));
	case EFenixCookSelectorInputFormat::Set:
		return FindPin(TEXT("CookedSet"));
	case EFenixCookSelectorInputFormat::Map:
		return FindPin(TEXT("CookedMap"));
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
