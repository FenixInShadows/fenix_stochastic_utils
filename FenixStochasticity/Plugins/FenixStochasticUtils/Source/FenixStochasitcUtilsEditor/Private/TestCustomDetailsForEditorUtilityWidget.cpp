#include "TestCustomDetailsForEditorUtilityWidget.h"
#include "MyActor.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SSlider.h"
#include "IPropertyUtilities.h"
#include "BlueprintEditor.h"
#include "Engine/SCS_Node.h" 
#include "Engine/InheritableComponentHandler.h"

void FTestCustomDetailsForEditorUtilityWidget::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& CustomCategory = DetailBuilder.EditCategory("ReplaceMaterial");
	CustomCategory.AddCustomRow(FText::FromString("ReplaceMaterial"))
		.ValueContent()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)
		[
			SNew(SSlider)
		];
}