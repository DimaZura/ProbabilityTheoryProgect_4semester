//
// Created by mac on 23.05.2026.
//

#include "Models.h"



MxDxResult::MxDxResult(double InMx, double InDx) {
    Mx = InMx;
    Dx = InDx;
}



TestResult::TestResult(double InTestValue, double InCriticalValue, double InAlpha) {
    TestValue = InTestValue;
    CriticalValue = InCriticalValue;
    Alpha = InAlpha;
}



