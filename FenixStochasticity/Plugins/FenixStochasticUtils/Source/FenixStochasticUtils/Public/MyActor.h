// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

USTRUCT(BlueprintType)
struct FENIXSTOCHASTICUTILS_API FMyStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Label = TEXT("DefaultName");

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	double Value = 0.0;
};

UCLASS()
class FENIXSTOCHASTICUTILS_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom")
	bool bCanEditMyData = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (DisplayName="MyData1", EditCondition="bCanEditMyData"))
	FMyStruct MyStructData1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (DisplayName="MyData2", EditCondition="bCanEditMyData", EditConditionHides))
	FMyStruct MyStructData2;

	UPROPERTY(EditAnywhere)
	double MyTestValue = 0.1234567;

	UPROPERTY(EditAnywhere, Category = "My Int")
	int32 MyTestInt = 0;

	UPROPERTY(EditInstanceOnly, Category = "My Int")
	int32 MyTestIntNewValue = 0;

	UFUNCTION(CallInEditor, Category = "My Int")
	void ModifyMyTestInt();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
