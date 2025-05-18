#include "TestCustomDetails.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SSlider.h"
#include "Engine/SimpleConstructionScript.h"
#include "Engine/SCS_Node.h" 
#include "IPropertyUtilities.h"

void FTestCustomDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	DetailBuilder.GetObjectsBeingCustomized(SelectedObjects);
	NotifyHook = DetailBuilder.GetPropertyUtilities()->GetNotifyHook();

	DetailBuilder.HideCategory("TransformCommon");

	double MaxScale = 0.0;
	for (TWeakObjectPtr Object : SelectedObjects)
	{
		if (AActor* Actor = Cast<AActor>(Object.Get()))
		{
			if (Actor->HasAnyFlags(RF_ClassDefaultObject))  // CDO's root component is not created atm, need to get from templates in blueprint 
			{
				UClass* ActorClass = Actor->GetClass();
				UBlueprint* Blueprint = Cast<UBlueprint>(ActorClass->ClassGeneratedBy);
				const TArray<USCS_Node*>& RootNodes = Blueprint->SimpleConstructionScript->GetRootNodes();
				for (USCS_Node* Node : RootNodes)
				{
					USceneComponent* RootComp = Cast<USceneComponent>(Node->ComponentTemplate.Get());
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
				UClass* ActorClass = Actor->GetClass();
				UBlueprint* Blueprint = Cast<UBlueprint>(ActorClass->ClassGeneratedBy);
				const TArray<USCS_Node*>& RootNodes = Blueprint->SimpleConstructionScript->GetRootNodes();
				for (USCS_Node* Node : RootNodes)
				{
					USceneComponent* RootComp = Cast<USceneComponent>(Node->ComponentTemplate.Get());
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
