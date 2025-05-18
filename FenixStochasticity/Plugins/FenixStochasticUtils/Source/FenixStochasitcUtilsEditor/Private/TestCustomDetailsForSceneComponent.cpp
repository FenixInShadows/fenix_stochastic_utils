#include "TestCustomDetailsForSceneComponent.h"
#include "MyActor.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SSlider.h"
#include "IPropertyUtilities.h"
#include "BlueprintEditor.h"
#include "Engine/SCS_Node.h" 

void FTestCustomDetailsForSceneComponent::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> SelectedObjects;
	DetailBuilder.GetObjectsBeingCustomized(SelectedObjects);
	for (TWeakObjectPtr Object : SelectedObjects)
	{
		if (USceneComponent* SceneComp = Cast<USceneComponent>(Object.Get()))
		{
			bool NeedHide = false;
			if (Object->HasAnyFlags(RF_ArchetypeObject))
			{
				FNotifyHook* NotifyHook = DetailBuilder.GetPropertyUtilities()->GetNotifyHook();
				UBlueprint* Blueprint = static_cast<FBlueprintEditor*>(NotifyHook)->GetBlueprintObj();
				if (Blueprint->GeneratedClass->IsChildOf(AMyActor::StaticClass()))
				{
					const TArray<USCS_Node*>& RootNodes = Blueprint->SimpleConstructionScript->GetRootNodes();
					for (USCS_Node* Node : RootNodes)
					{
						USceneComponent* RootComp = Cast<USceneComponent>(Node->ComponentTemplate.Get());
						if (RootComp == Object)
						{
							NeedHide = true;
							break;
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