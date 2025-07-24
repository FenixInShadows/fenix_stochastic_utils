// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyEditorFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FENIXSTOCHASTICUTILSEDITOR_API UMyEditorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void MarkPackageDirty(UObject* Object);

	UFUNCTION(BlueprintCallable, meta = (Category = "SlateNotifications"))
	static void NotifySuccess(const FText& NotificationText, const FString& HyperlinkURL, const FText& HyperlinkText);
};
