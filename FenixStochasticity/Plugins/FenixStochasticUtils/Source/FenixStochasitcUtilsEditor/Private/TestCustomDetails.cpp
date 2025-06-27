#include "TestCustomDetails.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SSlider.h"
#include "Engine/SimpleConstructionScript.h"
#include "Engine/SCS_Node.h"
#include "Engine/InheritableComponentHandler.h"
#include "IPropertyUtilities.h"

void FTestCustomDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> SelectedObjects;
	DetailBuilder.GetObjectsBeingCustomized(SelectedObjects);

	DetailBuilder.HideCategory("TransformCommon");


	TArray<UObject*> RootComponents;

	double MaxScale = 0.0;
	for (TWeakObjectPtr Object : SelectedObjects)
	{
		if (AActor* Actor = Cast<AActor>(Object.Get()))
		{
			if (Actor->HasAnyFlags(RF_ClassDefaultObject))  // CDO's root component is not created atm, need to get from templates in blueprint 
			{
				UBlueprintGeneratedClass* ActorGeneratedClass = Cast<UBlueprintGeneratedClass>(Actor->GetClass());
				USceneComponent* RootComp = GetRootComponentTemplate(ActorGeneratedClass);
				if (RootComp)
				{
					RootComponents.Add(RootComp);
					FVector Scale = RootComp->GetRelativeScale3D();
					for (int i = 0; i < 3; i++)
					{
						double SubScale = Scale[i];
						if (SubScale > MaxScale)
						{
							MaxScale = SubScale;
						}
					}
				}
			}
			else
			{
				USceneComponent* RootComp = Actor->GetRootComponent();
				if (RootComp)  // when the actor's blueprint is compling and the instance in the map is being selected at the same time, this can be null
				{
					RootComponents.Add(RootComp);
					FVector Scale = Actor->GetActorRelativeScale3D();
					for (int i = 0; i < 3; i++)
					{
						double SubScale = Scale[i];
						if (SubScale > MaxScale)
						{
							MaxScale = SubScale;
						}
					}
				}
			}
		}
	}

	IDetailCategoryBuilder& CustomCategory = DetailBuilder.EditCategory("CustomScaling", FText::FromString("Scale"), ECategoryPriority::Important);
	CustomCategory.SetSortOrder(0);
	CustomCategory.AddCustomRow(FText::FromString("CustomScaling"))
		.ValueContent()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)
		[
			SNew(SSlider)
				.Value(MaxScale)
				.OnValueChanged(this, &FTestCustomDetails::OnValueChanged)
		];

	if (!RootComponents.IsEmpty())
	{
		RootScalePropHandle = DetailBuilder.AddObjectPropertyData(RootComponents, USceneComponent::GetRelativeScale3DPropertyName());

		TSharedPtr<IPropertyHandle> VisiblePropHandle = DetailBuilder.AddObjectPropertyData(RootComponents, USceneComponent::GetVisiblePropertyName());
		VisiblePropHandle->SetPropertyDisplayName(FText::FromString("Root Visibility"));
		DetailBuilder.AddPropertyToCategory(VisiblePropHandle);
	}
}

void FTestCustomDetails::OnValueChanged(const float NewValue) const
{
	FString FormattedString = FString::Printf(TEXT("(X=%.3f,Y=%.3f,Z=%.3f)"), NewValue, NewValue, NewValue);
	RootScalePropHandle->SetValueFromFormattedString(FormattedString);
}

USceneComponent* FTestCustomDetails::GetRootComponentTemplate(UBlueprintGeneratedClass* ActorGeneratedClass) const
{
	if (UInheritableComponentHandler* InheritableComponentHandler = ActorGeneratedClass->GetInheritableComponentHandler(false))  // this means this is a child of a blueprint actor
	{
		for (TArray<FComponentOverrideRecord>::TIterator It = InheritableComponentHandler->CreateRecordIterator(); It; ++It)
		{
			USCS_Node* Node = It->ComponentKey.FindSCSNode();
			if (Node && Node->IsRootNode() && Node->ParentComponentOrVariableName == NAME_None) // check for null node during iteration, for cases like the parent class removed some component but haven't sync to child class yet
			{
				USceneComponent* RootComp = Cast<USceneComponent>(It->ComponentTemplate.Get());
				if (RootComp)
				{
					return RootComp;
				}
			}
		}
	}
	else  // this means it is the top most blueprint actor class (it's parent would be C++ actor class)
	{
		const TArray<USCS_Node*>& RootNodes = ActorGeneratedClass->SimpleConstructionScript->GetRootNodes();
		for (USCS_Node* Node : RootNodes)
		{
			if (Node->ParentComponentOrVariableName == NAME_None)
			{
				USceneComponent* RootComp = Cast<USceneComponent>(Node->ComponentTemplate.Get());
				if (RootComp)
				{
					return RootComp;
				}
			}
		}
	}

	return nullptr;
}
