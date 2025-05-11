#include "TestGenericDetailCustomization.h"
#include "MyActor.h"
#include "UObject/UnrealType.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SSlider.h"

void FTestGenericDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{	
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	TArray<TWeakObjectPtr<UObject>> SelectedObjects;
	DetailLayout.GetObjectsBeingCustomized(SelectedObjects);
	if (SelectedObjects.Num() == 1)
	{
		UBlueprint* BlueprintPtr = Cast<UBlueprint>(SelectedObjects[0].Get());
		if (BlueprintPtr && BlueprintPtr->GeneratedClass->IsChildOf(AMyActor::StaticClass()))
		{
			// for maintain the regular order, create and sort the category that needs to be before the custom categories here
			IDetailCategoryBuilder& ClassOptionCategory = DetailLayout.EditCategory("ClassOptions");
			ClassOptionCategory.SetSortOrder(0);

			IDetailCategoryBuilder& CustomCategory = DetailLayout.EditCategory("CustomClassSetting", FText::FromString("Test Class Setting"));
			CustomCategory.SetSortOrder(1);
			CustomCategory.AddCustomRow(FText::FromString("CustomClassSetting"))
				.ValueContent()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Fill)
				[
					SNew(SSlider)
				];
		}

		UPropertyWrapper* PropertyWrapperPtr = Cast<UPropertyWrapper>(SelectedObjects[0].Get());
		if (PropertyWrapperPtr)
		{
			FStructProperty* StructPropertyPtr = CastField<FStructProperty>(PropertyWrapperPtr->GetProperty());
			if (StructPropertyPtr && StructPropertyPtr->Struct->IsChildOf(FMyStruct::StaticStruct()))
			{
				IDetailCategoryBuilder& CustomCategory = DetailLayout.EditCategory("CustomMyStruct", FText::FromString("Test My Struct"));
				CustomCategory.SetSortOrder(0);
				CustomCategory.AddCustomRow(FText::FromString("CustomMyStruct"))
					.ValueContent()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Fill)
					[
						SNew(SSlider)
					];
			}
		}

	}
}

