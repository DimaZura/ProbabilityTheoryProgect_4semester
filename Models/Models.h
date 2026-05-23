//
// Created by mac on 23.05.2026.
//

#ifndef PROJECT_MODELS_H
#define PROJECT_MODELS_H
#include <map>

struct MxDxResult {
    double Mx;
    double Dx;

    MxDxResult() = default;
    MxDxResult(double InMx, double InDx);
};

struct AnalyticsMxDxResult {
    MxDxResult Th;
    std::map<int, MxDxResult> Emp;
};


struct TestResult {
    double TestValue;
    double CriticalValue;
    double Alpha;

    TestResult() = default;
    TestResult(double InTestValue, double InCriticalValue, double InAlpha);
};

struct AnalyticsTestsResult {
    std::map<int, TestResult> TestsResults;
};




#endif //PROJECT_MODELS_H
