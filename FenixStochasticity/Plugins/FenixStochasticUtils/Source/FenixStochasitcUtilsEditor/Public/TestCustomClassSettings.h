#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class IDetailLayoutBuilder;

class FTestCustomClassSettings : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance() {return MakeShareable(new FTestCustomClassSettings);}

protected:
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
	void OnValueChanged(float NewValue) const;
};
