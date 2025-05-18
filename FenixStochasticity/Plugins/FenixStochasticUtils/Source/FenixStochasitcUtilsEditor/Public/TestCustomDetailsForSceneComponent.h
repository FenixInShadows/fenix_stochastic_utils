#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class FTestCustomDetailsForSceneComponent : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance() { return MakeShareable(new FTestCustomDetailsForSceneComponent); }

protected:
	virtual void CustomizeDetails(class IDetailLayoutBuilder& DetailBuilder) override;
};
