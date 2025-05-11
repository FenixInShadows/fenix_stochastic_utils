#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class IDetailLayoutBuilder;

class FTestCustomPropertyType : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance() { return MakeShareable(new FTestCustomPropertyType); }

protected:
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	void OnValueChanged(float NewValue) const;
};
