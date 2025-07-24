// Copyright 2025, Tiannan Chen, All rights reserved.


#include "MyEditorFunctionLibrary.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

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
