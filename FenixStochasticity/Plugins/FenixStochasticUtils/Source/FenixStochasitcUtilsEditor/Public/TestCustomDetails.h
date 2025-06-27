#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class FTestCustomDetails : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance() { return MakeShareable(new FTestCustomDetails); }

protected:
	virtual void CustomizeDetails(class IDetailLayoutBuilder& DetailBuilder) override;
	void OnValueChanged(float NewValue) const;
	USceneComponent* GetRootComponentTemplate(UBlueprintGeneratedClass* ActorGeneratedClass) const;

	TSharedPtr<IPropertyHandle> RootScalePropHandle;
};
