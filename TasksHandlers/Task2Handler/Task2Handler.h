//
// Created by mac on 16.05.2026.
//

#ifndef PROJECT_TASK2HANDLER_H
#define PROJECT_TASK2HANDLER_H
#include <map>

#include "../../Models/Models.h"


class Task2Handler {

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

    static AnalyticsTestsResult PearsonTests;

    //
    //  Выполнение задания
    //

    static void Process(int SelectionSize);

    // Вычисление критерия согласия х2 Пирсона
    static double CalculatePearsonTest(std::vector<double> Selection, int n, double p);

};


#endif //PROJECT_TASK2HANDLER_H
