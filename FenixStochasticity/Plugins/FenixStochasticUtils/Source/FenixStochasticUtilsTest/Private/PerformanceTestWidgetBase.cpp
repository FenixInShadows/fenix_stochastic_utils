// Copyright 2025, Tiannan Chen, All rights reserved.


#include "PerformanceTestWidgetBase.h"

void UPerformanceTestWidgetBase::TestPerformance()
{
    double StartTime = FPlatformTime::Seconds();
    for (int i = 0; i < NumIterations; i++)
    {
        TestFunction.ExecuteIfBound();
    }
    double ExecuteTime = FPlatformTime::Seconds() - StartTime;

    SetResultTotalTime(ExecuteTime);
}
