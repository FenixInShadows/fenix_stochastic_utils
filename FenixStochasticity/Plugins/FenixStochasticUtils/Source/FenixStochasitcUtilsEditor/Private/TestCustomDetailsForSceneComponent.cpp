#include "TestCustomDetailsForSceneComponent.h"
#include "MyActor.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SSlider.h"
#include "IPropertyUtilities.h"
#include "BlueprintEditor.h"
#include "Engine/SCS_Node.h" 
#include "Engine/InheritableComponentHandler.h"

void FTestCustomDetailsForSceneComponent::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> SelectedObjects;
	DetailBuilder.GetObjectsBeingCustomized(SelectedObjects);
	for (TWeakObjectPtr Object : SelectedObjects)
	{
		if (USceneComponent* SceneComp = Cast<USceneComponent>(Object.Get()))
		{
			bool NeedHide = false;
			if (SceneComp->HasAnyFlags(RF_ArchetypeObject))
			{
				UBlueprintGeneratedClass* OuterGeneratedClass = Cast<UBlueprintGeneratedClass>(SceneComp->GetOuter());
				if (OuterGeneratedClass && OuterGeneratedClass->IsChildOf(AMyActor::StaticClass()))
				{
					if (SceneComp->HasAnyFlags(RF_InheritableComponentTemplate))
					{
						UInheritableComponentHandler* InheritableComponentHandler = OuterGeneratedClass->GetInheritableComponentHandler(false);
						FComponentKey ComponentKey = InheritableComponentHandler->FindKey(SceneComp);
						if (ComponentKey.IsValid())
						{
							if (USCS_Node* Node = ComponentKey.FindSCSNode())
							{
								if (Node->IsRootNode() && Node->ParentComponentOrVariableName == NAME_None)
								{
									NeedHide = true;
								}
							}
						}
					}
					else
					{
						const TArray<USCS_Node*>& RootNodes = OuterGeneratedClass->SimpleConstructionScript->GetRootNodes();
						for (USCS_Node* Node : RootNodes)
						{
							// here this is a must, because RF_InheritableComponentTemplate can be on non-inherited component on child, which can be a "fake" root
							if (Node->ParentComponentOrVariableName == NAME_None)
							{
								USceneComponent* RootComp = Cast<USceneComponent>(Node->ComponentTemplate.Get());
								if (RootComp == SceneComp)
								{
									NeedHide = true;
									break;
								}
							}
						}
					}
				}
			}
			else
			{
				AActor* Owner = SceneComp->GetOwner();
				if (Owner->GetClass()->IsChildOf(AMyActor::StaticClass()) && Owner->GetRootComponent() == SceneComp)
				{
					NeedHide = true;
				}
			}
			
			if (NeedHide)
			{
				DetailBuilder.HideCategory("TransformCommon");
				DetailBuilder.HideCategory("Transform");
			}
		}
	}
}