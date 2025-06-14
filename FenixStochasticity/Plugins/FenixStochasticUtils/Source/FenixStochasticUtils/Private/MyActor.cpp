// Copyright 2025, Tiannan Chen, All rights reserved.

#include "MyActor.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMyActor::ModifyMyTestInt()
{
	if (MyTestInt != MyTestIntNewValue)
	{
		if (GIsEditor)
		{
			FProperty* ChangedProperty = FindFProperty<FProperty>(GetClass(), GET_MEMBER_NAME_CHECKED(AMyActor, MyTestInt));
			PreEditChange(ChangedProperty);
			MyTestInt = MyTestIntNewValue;
			TArrayView<const UObject* const> ChangedObjects{ this };
			FPropertyChangedEvent PropertyChangedEvent(ChangedProperty, EPropertyChangeType::ValueSet, ChangedObjects);
			PostEditChangeProperty(PropertyChangedEvent);
		}
		else
		{
			MyTestInt = MyTestIntNewValue;
		}
	}
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

