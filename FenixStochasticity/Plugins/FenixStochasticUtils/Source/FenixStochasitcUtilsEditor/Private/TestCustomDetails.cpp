#include "TestCustomDetails.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SSlider.h"

void FTestCustomDetails::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	IDetailCategoryBuilder& CustomCategory = DetailLayout.EditCategory("Custom", FText::FromString("Test"), ECategoryPriority::Important);
	CustomCategory.SetSortOrder(0);
	CustomCategory.AddCustomRow(FText::FromString("Custom"))
		.ValueContent()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)
		[
			SNew(SSlider)
				.OnValueChanged(this, &FTestCustomDetails::OnValueChanged)
		];
}

void FTestCustomDetails::OnValueChanged(const float NewValue) const
{
}
