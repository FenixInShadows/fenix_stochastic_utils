#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class FTestCustomDetailsForEditorUtilityWidget : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance() { return MakeShareable(new FTestCustomDetailsForEditorUtilityWidget); }

protected:
	virtual void CustomizeDetails(class IDetailLayoutBuilder& DetailBuilder) override;
};
