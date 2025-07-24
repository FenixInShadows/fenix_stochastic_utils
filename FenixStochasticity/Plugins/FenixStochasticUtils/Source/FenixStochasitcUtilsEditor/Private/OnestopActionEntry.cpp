// Copyright 2025, Tiannan Chen, All rights reserved.


#include "OnestopActionEntry.h"

void UOnestopActionEntry::SelectAction()
{
	OnSelected.ExecuteIfBound();
}
