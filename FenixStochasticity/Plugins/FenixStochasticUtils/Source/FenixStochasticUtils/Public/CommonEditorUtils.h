// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FENIXSTOCHASTICUTILS_API CommonEditorUtils
{
public:
	CommonEditorUtils();
	~CommonEditorUtils();

	static void ChangeToWildCardPinType(FEdGraphPinType& PinType);

	static void CopyPinTypeCategoryInfo(FEdGraphPinType& PinType, const FEdGraphPinType& SrcPinType);

	static void CopyPinTypeAndValueTypeInfo(FEdGraphPinType& PinType, const FEdGraphPinType& SrcPinType);

	static void ChangePinCategoryToDouble(FEdGraphPinType& PinType);

	static void ChangePinCategoryToWeightOrProbEntry(FEdGraphPinType& PinType);

	static void ChangePinCategoryToWildcard(FEdGraphPinType& PinType);

	static void ChangePinCategoryToName(FEdGraphPinType& PinType);

	static void ChangePinValueCategoryToDouble(FEdGraphTerminalType& PinValueType);

	static void ChangePinValueCategoryToWeightOrProbEntry(FEdGraphTerminalType& PinValueType);

	static void ChangePinTypeToDoubleArray(FEdGraphPinType& PinType);

	static void ChangePinTypeToDoubleArrayRef(FEdGraphPinType& PinType);

	static void ChangePinTypeToWeightOrProbArrayRef(FEdGraphPinType& PinType);

	static void ChangePinTypeToDoubleMapRef(FEdGraphPinType& PinType);

	static void ChangePinTypeToWeightOrProbMapRef(FEdGraphPinType& PinType);

	static void ChangePinTypeToDataTablePtr(FEdGraphPinType& PinType);

	static void ChangePinTypeToCookedSelectorDistribution(FEdGraphPinType& PinType);

	static void ChangePinTypeToCookedSelectorDistributionRef(FEdGraphPinType& PinType);

	static void OnPinConnectionUpdatedWithCategoryInfoSync(UEdGraphPin* Pin, UEdGraphPin* SyncedPin);

	/** Return whether pin info is updated (e.g. if the Pin has no connection then there's nothing to update). */
	static bool PostPinConnectionReconstructionWithCategoryInfoSync(UEdGraphPin* Pin, UEdGraphPin* SyncedPin);
};
