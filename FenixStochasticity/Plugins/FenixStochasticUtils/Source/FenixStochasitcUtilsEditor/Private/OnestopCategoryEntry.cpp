// Copyright 2025, Tiannan Chen, All rights reserved.


#include "OnestopCategoryEntry.h"

void UOnestopCategoryEntry::SelectCategory()
{
	OnSelected.ExecuteIfBound(this);
}
