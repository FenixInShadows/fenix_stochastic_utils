#include "TestCustomPropertyType.h"
#include "MyActor.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Widgets/Input/SSlider.h"

void FTestCustomPropertyType::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	HeaderRow
		.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(SSlider)
				.OnValueChanged(this, &FTestCustomPropertyType::OnValueChanged)
		];
}

void FTestCustomPropertyType::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	ChildBuilder.AddProperty(PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMyStruct, Label)).ToSharedRef());
	ChildBuilder.AddProperty(PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMyStruct, Value)).ToSharedRef());
}

void FTestCustomPropertyType::OnValueChanged(const float NewValue) const
{
}
