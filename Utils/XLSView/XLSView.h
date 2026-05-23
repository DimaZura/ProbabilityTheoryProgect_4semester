//
// Created by mac on 20.05.2026.
//

#ifndef PROJECT_XLSVIEW_H
#define PROJECT_XLSVIEW_H
#include <map>

#include "../../Models/Models.h"


class XLSView {
public:

    // Сохранить таблицу сравнения математического ожидания и дисперсии
    static void SaveMxDxTable(AnalyticsMxDxResult MxDxResult, int taskNum);

    // сохранить статистический ряд непрерывной величины
    static void SaveStatisticalSeriesForContinuous(std::vector<double> EDF, int min, int max, int SelectionSize, int taskNum);

    // сохранить статистический ряд дискретной величины
    static void SaveStatisticalSeriesForDiscrete(std::vector<double> EDF, int min, int max, int SelectionSize, int taskNum);

    // сохранить значения оценок распределения
    static void SaveTestResult(AnalyticsTestsResult Result, int taskNum);
};


#endif //PROJECT_XLSVIEW_H
