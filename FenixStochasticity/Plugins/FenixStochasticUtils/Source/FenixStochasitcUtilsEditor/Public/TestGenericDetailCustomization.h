#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class IDetailLayoutBuilder;

class FTestGenericDetailCustomization : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance() {return MakeShareable(new FTestGenericDetailCustomization);}

protected:
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
};
