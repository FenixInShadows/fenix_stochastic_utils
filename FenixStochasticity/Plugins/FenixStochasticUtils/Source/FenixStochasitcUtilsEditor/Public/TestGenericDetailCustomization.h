#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class FTestGenericDetailCustomization : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance() {return MakeShareable(new FTestGenericDetailCustomization);}

protected:
	virtual void CustomizeDetails(class IDetailLayoutBuilder& DetailBuilder) override;
};
