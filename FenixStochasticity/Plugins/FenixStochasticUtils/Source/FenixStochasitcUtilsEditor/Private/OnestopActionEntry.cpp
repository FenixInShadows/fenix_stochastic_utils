// Copyright 2025, Tiannan Chen, All rights reserved.


#include "OnestopActionEntry.h"

void UOnestopActionEntry::SelectAction()
{
	OnSelected.ExecuteIfBound();
	PostSelected.ExecuteIfBound(this);
}

void UOnestopActionEntry::SwitchIsBookmarked()
{
	UpdateIsBookmarked(!bIsBookmarked);
}

void UOnestopActionEntry::UpdateIsBookmarked(const bool bBookmarked)
{
	bIsBookmarked = bBookmarked;
	SetBookmarkState(bBookmarked);
	PostIsBookmarkedUpdate.ExecuteIfBound(this, bBookmarked);
}

void UOnestopActionEntry::InitIsBookmarked(const bool bBookmarked)
{
	bIsBookmarked = bBookmarked;
	SetBookmarkState(bBookmarked);
}

void UOnestopActionEntry::InitIsBookmarkable(const bool bBookmarkable)
{
	if (!bBookmarkable)
	{
		InitIsBookmarked(false);
	}

	SetBookmarkableState(bBookmarkable);
}

