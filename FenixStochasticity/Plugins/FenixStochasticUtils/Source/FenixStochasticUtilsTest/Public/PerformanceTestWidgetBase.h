// Copyright 2025, Tiannan Chen, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"

#include "PerformanceTestWidgetBase.generated.h"

DECLARE_DYNAMIC_DELEGATE(FTestFuncDelegate);

/**
 * 
 */
UCLASS()
class FENIXSTOCHASTICUTILSTEST_API UPerformanceTestWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumIterations = 100000;

protected:
	UFUNCTION(BlueprintCallable)
	void SetTestFunction(const FTestFuncDelegate& Func)
	{
		TestFunction = Func;
	}

	UFUNCTION(BlueprintCallable)
	void TestPerformance();

	UFUNCTION(BlueprintImplementableEvent)
	void SetResultTotalTime(double Time);

private:
	FTestFuncDelegate TestFunction;
};
