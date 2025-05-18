#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class FTestCustomDetails : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance() { return MakeShareable(new FTestCustomDetails); }

protected:
	TArray<TWeakObjectPtr<UObject>> SelectedObjects;
	FNotifyHook* NotifyHook;

	virtual void CustomizeDetails(class IDetailLayoutBuilder& DetailBuilder) override;
	void OnValueChanged(float NewValue) const;
};
