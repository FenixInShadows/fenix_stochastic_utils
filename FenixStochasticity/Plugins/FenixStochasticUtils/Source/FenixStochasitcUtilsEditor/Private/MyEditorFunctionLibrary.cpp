// Copyright 2025, Tiannan Chen, All rights reserved.


#include "MyEditorFunctionLibrary.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "HAL/PlatformFileManager.h"
#include "AssetToolsModule.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#include "Subsystems/EditorAssetSubsystem.h"

void UMyEditorFunctionLibrary::MarkPackageDirty(UObject* Object)
{
	Object->MarkPackageDirty();
}

void UMyEditorFunctionLibrary::NotifySuccess(const FText& NotificationText, const FString& HyperlinkURL, const FText& HyperlinkText)
{
	FNotificationInfo Info(NotificationText);

	Info.ExpireDuration = 3.0f;
	Info.Image = FCoreStyle::Get().GetBrush("Icons.SuccessWithColor");

	if (!HyperlinkURL.IsEmpty())
	{
		Info.HyperlinkText = HyperlinkText;
		Info.Hyperlink = FSimpleDelegate::CreateLambda([HyperlinkURL]()
			{
				FPlatformProcess::LaunchURL(*HyperlinkURL, nullptr, nullptr);
			});
	}

	FSlateNotificationManager::Get().AddNotification(Info);
}

void UMyEditorFunctionLibrary::CreateDirectoryTree(const FString& DirSoftPath, bool bRefreshRegistry)
{
	FString AbsolutePath;
	if (!FPackageName::TryConvertLongPackageNameToFilename(DirSoftPath, AbsolutePath)) {
		UE_LOG(LogTemp, Error, TEXT("Invalid virtual path: %s"), *DirSoftPath);
		return;
	}

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	PlatformFile.CreateDirectoryTree(*AbsolutePath);

	if (bRefreshRegistry)
	{
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		AssetRegistryModule.Get().ScanPathsSynchronous({ DirSoftPath }, true);
	}
}

UObject* UMyEditorFunctionLibrary::CreateAsset(const FString& AssetName, const FString& DirSoftPath, UClass* AssetClass, UFactory* Factory, bool bSaveAsset)
{
	UObject* NewAsset = FAssetToolsModule::GetModule().Get().CreateAsset(AssetName, DirSoftPath, AssetClass, Factory);
	
	if (bSaveAsset)
	{
		SaveAsset(NewAsset);
	}
	
	return NewAsset;
}

UObject* UMyEditorFunctionLibrary::LoadAsset(const FString& AssetName, const FString& DirSoftPath)
{
	const FString AssetPath = DirSoftPath / FString::Format(TEXT("{0}.{0}"), { AssetName });
	return GEditor->GetEditorSubsystem<UEditorAssetSubsystem>()->LoadAsset(AssetPath);
}

void UMyEditorFunctionLibrary::SaveAsset(UObject* Asset)
{
	if (Asset)
	{
		UPackage* Package = Asset->GetPackage();
		FString PackageFileName = FPackageName::LongPackageNameToFilename(
			Package->GetName(),
			FPackageName::GetAssetPackageExtension()
		);

		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
		UPackage::SavePackage(Package, nullptr, *PackageFileName, SaveArgs);
	}
}
