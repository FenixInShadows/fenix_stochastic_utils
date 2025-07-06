// Copyright 2025, Tiannan Chen, All rights reserved.


#include "MyEditorFunctionLibrary.h"

void UMyEditorFunctionLibrary::MarkPackageDirty(UObject* Object)
{
	Object->MarkPackageDirty();
}
