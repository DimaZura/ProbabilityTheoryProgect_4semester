//
// Created by mac on 16.05.2026.
//

#ifndef PROJECT_TASK1HANDLER_H
#define PROJECT_TASK1HANDLER_H
#include <map>

#include "../../Models/Models.h"


class Task1Handler {

public:

    static void Run();

protected:

    static  constexpr bool Debug = false;

    //
    //  Данные математического ожидания и дисперсии
    //

    static AnalyticsMxDxResult AnalyticsMxDxResult;

    //
    //  Данные оценок
    //

    static AnalyticsTestsResult KolmogorovTests;


    //
    //  Выполнение задания
    //

    static void Process(int SelectionSize, double Alpha);

    // Вычисление критерия согласия Колмагорова-Смирнова
    static double CalculateKolmogorovTest(std::vector<double> Selection, int min, int max);

};


#endif //PROJECT_TASK1HANDLER_H
