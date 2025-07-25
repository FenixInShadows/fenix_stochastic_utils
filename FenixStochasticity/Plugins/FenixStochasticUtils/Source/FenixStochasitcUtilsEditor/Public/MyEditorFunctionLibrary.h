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

	static bool IsDirectoryExist(const FString DirSoftPath);
	static void CreateDirectory(const FString& DirSoftPath, bool bRefreshRegistry = true);
	static bool IsAssetExist(const FString& AssetName, const FString& DirSoftPath);
	static UObject* CreateAsset(const FString& AssetName, const FString& DirSoftPath, UClass* AssetClass, UFactory* Factory, bool bSaveAsset = true);
	static UObject* LoadAsset(const FString& AssetName, const FString& DirSoftPath);
	static void SaveAsset(UObject* Asset);
};
