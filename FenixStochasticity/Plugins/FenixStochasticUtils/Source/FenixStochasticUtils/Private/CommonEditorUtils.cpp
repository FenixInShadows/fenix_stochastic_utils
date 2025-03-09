// Copyright 2025, Tiannan Chen, All rights reserved.


#include "CommonEditorUtils.h"
#include "SelectorUtils.h"

CommonEditorUtils::CommonEditorUtils()
{
}

CommonEditorUtils::~CommonEditorUtils()
{
}


void CommonEditorUtils::ChangeToWildCardPinType(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = nullptr;
	PinType.bIsWeakPointer = false;
	PinType.bIsUObjectWrapper = false;
	PinType.bSerializeAsSinglePrecisionFloat = false;
}

void CommonEditorUtils::CopyPinTypeCategoryInfo(FEdGraphPinType& PinType, const FEdGraphPinType& SrcPinType)
{
	PinType.PinCategory = SrcPinType.PinCategory;
	PinType.PinSubCategory = SrcPinType.PinSubCategory;
	PinType.PinSubCategoryObject = SrcPinType.PinSubCategoryObject;
	PinType.bIsWeakPointer = SrcPinType.bIsWeakPointer;
	PinType.bIsUObjectWrapper = SrcPinType.bIsUObjectWrapper;
	PinType.bSerializeAsSinglePrecisionFloat = SrcPinType.bSerializeAsSinglePrecisionFloat;
}

void CommonEditorUtils::CopyPinTypeAndValueTypeInfo(FEdGraphPinType& PinType, const FEdGraphPinType& SrcPinType)
{
	PinType.PinCategory = SrcPinType.PinCategory;
	PinType.PinSubCategory = SrcPinType.PinSubCategory;
	PinType.PinSubCategoryObject = SrcPinType.PinSubCategoryObject;
	PinType.bIsWeakPointer = SrcPinType.bIsWeakPointer;
	PinType.bIsUObjectWrapper = SrcPinType.bIsUObjectWrapper;
	PinType.bSerializeAsSinglePrecisionFloat = SrcPinType.bSerializeAsSinglePrecisionFloat;
	PinType.PinValueType = SrcPinType.PinValueType;
}

void CommonEditorUtils::ChangePinCategoryToDouble(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Real;
	PinType.PinSubCategory = UEdGraphSchema_K2::PC_Double;
	PinType.PinSubCategoryObject = nullptr;
}

void CommonEditorUtils::ChangePinCategoryToWeightOrProbEntry(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = FWeightOrProbEntry::StaticStruct();
}

void CommonEditorUtils::ChangePinCategoryToWildcard(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = nullptr;
}

void CommonEditorUtils::ChangePinCategoryToName(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Name;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = nullptr;
}

void CommonEditorUtils::ChangePinValueCategoryToDouble(FEdGraphTerminalType& PinValueType)
{
	PinValueType.TerminalCategory = UEdGraphSchema_K2::PC_Real;
	PinValueType.TerminalSubCategory = UEdGraphSchema_K2::PC_Double;
	PinValueType.TerminalSubCategoryObject = nullptr;
}

void CommonEditorUtils::ChangePinValueCategoryToWeightOrProbEntry(FEdGraphTerminalType& PinValueType)
{
	PinValueType.TerminalCategory = UEdGraphSchema_K2::PC_Struct;
	PinValueType.TerminalSubCategory = NAME_None;
	PinValueType.TerminalSubCategoryObject = FWeightOrProbEntry::StaticStruct();
}

void CommonEditorUtils::ChangePinTypeToDoubleArray(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Real;
	PinType.PinSubCategory = UEdGraphSchema_K2::PC_Double;
	PinType.PinSubCategoryObject = nullptr;
	PinType.ContainerType = EPinContainerType::Array;
	PinType.PinValueType = FEdGraphTerminalType();
}

void CommonEditorUtils::ChangePinTypeToDoubleArrayRef(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Real;
	PinType.PinSubCategory = UEdGraphSchema_K2::PC_Double;
	PinType.PinSubCategoryObject = nullptr;
	PinType.ContainerType = EPinContainerType::Array;
	PinType.PinValueType = FEdGraphTerminalType();
	PinType.bIsReference = true;
}

void CommonEditorUtils::ChangePinTypeToWeightOrProbArrayRef(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = FWeightOrProbEntry::StaticStruct();
	PinType.ContainerType = EPinContainerType::Array;
	PinType.PinValueType = FEdGraphTerminalType();
	PinType.bIsReference = true;
}

void CommonEditorUtils::ChangePinTypeToDoubleMapRef(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = nullptr;
	PinType.ContainerType = EPinContainerType::Map;
	FEdGraphTerminalType ValueType;
	ValueType.TerminalCategory = UEdGraphSchema_K2::PC_Real;
	ValueType.TerminalSubCategory = UEdGraphSchema_K2::PC_Double;
	PinType.PinValueType = ValueType;
	PinType.bIsReference = true;
}

void CommonEditorUtils::ChangePinTypeToWeightOrProbMapRef(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = nullptr;
	PinType.ContainerType = EPinContainerType::Map;
	FEdGraphTerminalType ValueType;
	ValueType.TerminalCategory = UEdGraphSchema_K2::PC_Struct;
	ValueType.TerminalSubCategoryObject = FWeightOrProbEntry::StaticStruct();
	PinType.PinValueType = ValueType;
	PinType.bIsReference = true;
}

void CommonEditorUtils::ChangePinTypeToDataTablePtr(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Object;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = UDataTable::StaticClass();
	PinType.ContainerType = EPinContainerType::None;
	PinType.PinValueType = FEdGraphTerminalType();
	PinType.bIsReference = false;
}

void CommonEditorUtils::ChangePinTypeToCookedSelectorDistribution(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = FCookedSelectorDistribution::StaticStruct();
	PinType.ContainerType = EPinContainerType::None;
	PinType.PinValueType = FEdGraphTerminalType();
}

void CommonEditorUtils::ChangePinTypeToCookedSelectorDistributionRef(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = FCookedSelectorDistribution::StaticStruct();
	PinType.ContainerType = EPinContainerType::None;
	PinType.PinValueType = FEdGraphTerminalType();
	PinType.bIsReference = true;
}

void CommonEditorUtils::OnPinConnectionUpdatedWithCategoryInfoSync(UEdGraphPin* Pin, UEdGraphPin* SyncedPin)
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
	else if (PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard)
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

bool CommonEditorUtils::PostPinConnectionReconstructionWithCategoryInfoSync(UEdGraphPin* Pin, UEdGraphPin* SyncedPin)
{
	if (!Pin->LinkedTo.IsEmpty() && Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard)
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
