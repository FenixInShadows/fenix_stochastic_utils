#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class IDetailLayoutBuilder;

class FTestCustomDetails : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance() { return MakeShareable(new FTestCustomDetails); }

protected:
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
	void OnValueChanged(float NewValue) const;
};
