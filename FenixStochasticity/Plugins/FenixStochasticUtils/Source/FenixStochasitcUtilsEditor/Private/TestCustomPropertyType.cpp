#include "TestCustomPropertyType.h"
#include "MyActor.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Widgets/Input/SSlider.h"

void FTestCustomPropertyType::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	StructPropertyHandle = PropertyHandle;

	TArray<void*> RawStructData;
	StructPropertyHandle->AccessRawData(RawStructData);
	FMyStruct* MyStructPtr = static_cast<FMyStruct*>(RawStructData[0]);
	float InitValue = MyStructPtr->Value / 10.0f;


	HeaderRow
		.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(SSlider)
				.Value(InitValue)
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
	FString FormattedString = FString::Printf(TEXT("(Value=%.2f)"), NewValue * 10.0);
	StructPropertyHandle->SetValueFromFormattedString(FormattedString);
}
