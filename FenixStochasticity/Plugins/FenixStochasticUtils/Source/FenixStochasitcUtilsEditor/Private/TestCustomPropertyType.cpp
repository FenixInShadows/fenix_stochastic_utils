#include "TestCustomPropertyType.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SSlider.h"

void FTestCustomPropertyType::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	/*HeaderRow.NameContent()
		[
			SNew(STextBlock)
				.Text(FText::FromString("TestMyStructHeader"))
				.Font(IDetailLayoutBuilder::GetDetailFont())
		];*/
}

void FTestCustomPropertyType::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
}

void FTestCustomPropertyType::OnValueChanged(const float NewValue) const
{
}
