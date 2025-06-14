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
	NotifyHook = DetailBuilder.GetPropertyUtilities()->GetNotifyHook();
	DetailBuilder.GetObjectsBeingCustomized(SelectedObjects);

	DetailBuilder.HideCategory("TransformCommon");

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
}

void FTestCustomDetails::OnValueChanged(const float NewValue) const
{
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
					FProperty* ChangedProperty = RootComp->GetClass()->FindPropertyByName(RootComp->GetRelativeRotationPropertyName());
					NotifyHook->NotifyPreChange(ChangedProperty);

					FVector PreviousScale = RootComp->GetRelativeScale3D();
					FVector NewScale = FVector(NewValue, NewValue, NewValue);
					RootComp->SetRelativeScale3D(NewScale);

					TArray<UObject*> ArchetypeInstances;
					RootComp->GetArchetypeInstances(ArchetypeInstances);
					for (UObject* ArchetypeObj : ArchetypeInstances)
					{
						if (USceneComponent* ArchetypeSceneComp = Cast<USceneComponent>(ArchetypeObj))
						{
							if (ArchetypeSceneComp->GetRelativeScale3D() == PreviousScale)
							{
								ArchetypeSceneComp->SetRelativeScale3D(NewScale);
							}
						}
					}

					TArrayView<const UObject* const> ChangedObjects{ RootComp };
					FPropertyChangedEvent PropertyChangedEvent(ChangedProperty, EPropertyChangeType::ValueSet, ChangedObjects);
					NotifyHook->NotifyPostChange(PropertyChangedEvent, ChangedProperty);
				}
			}
			else
			{
				Actor->SetActorRelativeScale3D(FVector(NewValue, NewValue, NewValue));
				Actor->MarkPackageDirty();
			}
			GEditor->RedrawAllViewports();
		}
	}
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
