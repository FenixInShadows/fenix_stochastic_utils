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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0.0, ClamepMax=1.0))
	double Value = 0.0;
};

UCLASS()
class FENIXSTOCHASTICUTILS_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMyStruct MyStructData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
