// Copyright 2025, Tiannan Chen, All rights reserved.


#include "CommonDeveloperUtils.h"
#include "SelectorUtils.h"

CommonDeveloperUtils::CommonDeveloperUtils()
{
}

CommonDeveloperUtils::~CommonDeveloperUtils()
{
}


void CommonDeveloperUtils::ChangeToWildCardPinType(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = nullptr;
	PinType.bIsWeakPointer = false;
	PinType.bIsUObjectWrapper = false;
	PinType.bSerializeAsSinglePrecisionFloat = false;
}

void CommonDeveloperUtils::CopyPinTypeCategoryInfo(FEdGraphPinType& PinType, const FEdGraphPinType& SrcPinType)
{
	PinType.PinCategory = SrcPinType.PinCategory;
	PinType.PinSubCategory = SrcPinType.PinSubCategory;
	PinType.PinSubCategoryObject = SrcPinType.PinSubCategoryObject;
	PinType.bIsWeakPointer = SrcPinType.bIsWeakPointer;
	PinType.bIsUObjectWrapper = SrcPinType.bIsUObjectWrapper;
	PinType.bSerializeAsSinglePrecisionFloat = SrcPinType.bSerializeAsSinglePrecisionFloat;
}

void CommonDeveloperUtils::CopyPinTypeAndValueTypeInfo(FEdGraphPinType& PinType, const FEdGraphPinType& SrcPinType)
{
	PinType.PinCategory = SrcPinType.PinCategory;
	PinType.PinSubCategory = SrcPinType.PinSubCategory;
	PinType.PinSubCategoryObject = SrcPinType.PinSubCategoryObject;
	PinType.bIsWeakPointer = SrcPinType.bIsWeakPointer;
	PinType.bIsUObjectWrapper = SrcPinType.bIsUObjectWrapper;
	PinType.bSerializeAsSinglePrecisionFloat = SrcPinType.bSerializeAsSinglePrecisionFloat;
	PinType.PinValueType = SrcPinType.PinValueType;
}

void CommonDeveloperUtils::ChangePinCategoryToDouble(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Real;
	PinType.PinSubCategory = UEdGraphSchema_K2::PC_Double;
	PinType.PinSubCategoryObject = nullptr;
}

void CommonDeveloperUtils::ChangePinCategoryToWeightOrProbEntry(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = FWeightOrProbEntry::StaticStruct();
}

void CommonDeveloperUtils::ChangePinCategoryToWildcard(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = nullptr;
}

void CommonDeveloperUtils::ChangePinCategoryToName(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Name;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = nullptr;
}

void CommonDeveloperUtils::ChangePinValueCategoryToDouble(FEdGraphTerminalType& PinValueType)
{
	PinValueType.TerminalCategory = UEdGraphSchema_K2::PC_Real;
	PinValueType.TerminalSubCategory = UEdGraphSchema_K2::PC_Double;
	PinValueType.TerminalSubCategoryObject = nullptr;
}

void CommonDeveloperUtils::ChangePinValueCategoryToWeightOrProbEntry(FEdGraphTerminalType& PinValueType)
{
	PinValueType.TerminalCategory = UEdGraphSchema_K2::PC_Struct;
	PinValueType.TerminalSubCategory = NAME_None;
	PinValueType.TerminalSubCategoryObject = FWeightOrProbEntry::StaticStruct();
}

void CommonDeveloperUtils::ChangePinTypeToDoubleArray(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Real;
	PinType.PinSubCategory = UEdGraphSchema_K2::PC_Double;
	PinType.PinSubCategoryObject = nullptr;
	PinType.ContainerType = EPinContainerType::Array;
	PinType.PinValueType = FEdGraphTerminalType();
}

void CommonDeveloperUtils::ChangePinTypeToDoubleArrayRef(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Real;
	PinType.PinSubCategory = UEdGraphSchema_K2::PC_Double;
	PinType.PinSubCategoryObject = nullptr;
	PinType.ContainerType = EPinContainerType::Array;
	PinType.PinValueType = FEdGraphTerminalType();
	PinType.bIsReference = true;
}

void CommonDeveloperUtils::ChangePinTypeToWeightOrProbArrayRef(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = FWeightOrProbEntry::StaticStruct();
	PinType.ContainerType = EPinContainerType::Array;
	PinType.PinValueType = FEdGraphTerminalType();
	PinType.bIsReference = true;
}

void CommonDeveloperUtils::ChangePinTypeToDoubleMapRef(FEdGraphPinType& PinType)
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

void CommonDeveloperUtils::ChangePinTypeToWeightOrProbMapRef(FEdGraphPinType& PinType)
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

void CommonDeveloperUtils::ChangePinTypeToDataTablePtr(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Object;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = UDataTable::StaticClass();
	PinType.ContainerType = EPinContainerType::None;
	PinType.PinValueType = FEdGraphTerminalType();
	PinType.bIsReference = false;
}

void CommonDeveloperUtils::ChangePinTypeToCookedSelectorDistribution(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = FCookedSelectorDistribution::StaticStruct();
	PinType.ContainerType = EPinContainerType::None;
	PinType.PinValueType = FEdGraphTerminalType();
}

void CommonDeveloperUtils::ChangePinTypeToCookedSelectorDistributionRef(FEdGraphPinType& PinType)
{
	PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
	PinType.PinSubCategory = NAME_None;
	PinType.PinSubCategoryObject = FCookedSelectorDistribution::StaticStruct();
	PinType.ContainerType = EPinContainerType::None;
	PinType.PinValueType = FEdGraphTerminalType();
	PinType.bIsReference = true;
}

void CommonDeveloperUtils::OnPinConnectionUpdatedWithCategoryInfoSync(UEdGraphPin* Pin, UEdGraphPin* SyncedPin)
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

bool CommonDeveloperUtils::PostPinConnectionReconstructionWithCategoryInfoSync(UEdGraphPin* Pin, UEdGraphPin* SyncedPin)
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
