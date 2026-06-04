//
// Created by mac on 16.05.2026.
//

#include "Task1Handler.h"

#include <iostream>
#include <vector>
#include <ostream>
#include <set>

#include "../../Utils/Generation/Generation.h"
#include "../../Utils/View/View.h"
#include <matplot/matplot.h>

#include "../../Utils/Converter/Converter.h"
#include "../../Utils/Math/Math.h"
#include "../../Utils/XLSView/XLSView.h"


//
//  Инициализация статических полей
//

AnalyticsMxDxResult Task1Handler::AnalyticsMxDxResult;
AnalyticsTestsResult Task1Handler::KolmogorovTests;


void Task1Handler::Run() {
    Process(25, 0.2);
    Process(100, 0.2);
    Process(500, 0.3);
    Process(1000, 0.4);
    // Process(10000000, 0.5);

    // сохранение таблицы расчетных данных
    XLSView::SaveMxDxTable(AnalyticsMxDxResult, 1);

    // сохранение таблицы оценок
    XLSView::SaveTestResult(KolmogorovTests, 1);
}

// Выполнение задания
void Task1Handler::Process(int SelectionSize, double Alpha) {

    //
    //  Начальные параметры
    //

    // параметры распределения
    int min = 2;
    int max = 6;

    // определение числа сегментов
    int CountSegments = std::log(SelectionSize)*3.32;
    if (CountSegments > 20) {
        CountSegments = 20;
    }

    //
    //  Генерация выборки
    //

    std::vector<double> Selection = Generation::CreateASelectionByRealDistribution(SelectionSize, min, max);

    //
    //  Сохранение гистограммы распределения
    //

    View::SaveHist(Selection,min, max, CountSegments, 1, std::to_string(SelectionSize) + "Hist");

    //
    //  Сохранение эмпирической функции распределения
    //

    View::SaveEmpiricalDistributionFunctionOfRealDistribution(Selection, max, min, CountSegments, 1, std::to_string(SelectionSize) + "EDF" );

    //
    //  Вычисление математического ожидания и Дисперсии
    //

    //  --- Эмпирические значения ---

    // ![Математическое ожидание](../../Images/Comments/8.png)
    double EmpMx = Math::CalculateExpectedValue(Selection);
    // ![Дисперсия](../../Images/Comments/9.png)
    double EmpDx = Math::CalculateVariance(Selection);

    MxDxResult EmpMxDxResult(EmpMx, EmpDx);
    AnalyticsMxDxResult.Emp.insert({SelectionSize, EmpMxDxResult});

    //  --- Теоретические значения ---

    // ![Математическое ожидание](../../Images/Comments/6.png)
    AnalyticsMxDxResult.Th.Mx = double(min+max)/2;
    // ![Дисперсия](../../Images/Comments/7.png)
    AnalyticsMxDxResult.Th.Dx = std::pow(double(max-min), 2) / 12;

    //
    //  Сохранение Статистического ряда
    //

    std::vector<double> EDF = Converter::ToEDF(Selection, min, max, SelectionSize, CountSegments);

    XLSView::SaveStatisticalSeriesForContinuous(EDF, min, max, SelectionSize, 1);

    //
    //  Определение критерия согласия λ-Колмагорова
    //

    // --- определение критического значения критерия ---

    std::map<double, double> AlphaToLambdaCrit {
            {0.2, 1.07},
            {0.3, 0.97},
            {0.4, 0.89},
            {0.5, 0.83}
    };
    double LambdaCrit = AlphaToLambdaCrit.at(Alpha);

    // Вычисления критерия
    double TestValue = CalculateKolmogorovTest(Selection, min, max);

    // сохранение

    TestResult TestResult(TestValue, LambdaCrit, Alpha);
    KolmogorovTests.TestsResults.insert({SelectionSize, TestResult});

}

// Производит вычисление критерия Колмагорова
double Task1Handler::CalculateKolmogorovTest(std::vector<double> Selection, int min, int max) {

    double Dmax = 0.0;
    int selection_size = Selection.size();


    if (selection_size == 0) return 0.0;

    // Сортировка выборки
    std::sort(Selection.begin(), Selection.end());

    // --- ![Формула поиска максимальной дельты](../../Images/Comments/1.png)
    // Идем строго по элементам выборки
    for (int i = 0; i < selection_size; i++) {

        // --- ![Формула функции равномерного распределения](../../Images/Comments/5.png)
        // Теоретическое DF в точке наблюдения
        //  теоретическая доля элементов меньших чем Selection[i]
        double ThDFVal = Math::TheoryRealDistribution(Selection[i], min, max);

        // --- ![Формула эмпирической функции распределения](../../Images/Comments/4.png)
        // Эмпирическое значение DF
        //  эмпирическая доля элементов которые меньше Selection[i]
        double EDFBefore = (double)i / selection_size;
        double EDFAfter  = (double)(i + 1) / selection_size;

        // Поиск дельт
        double DiffBefore = std::abs(EDFBefore - ThDFVal);
        double DiffAfter  = std::abs(EDFAfter - ThDFVal);

        if (Debug) {
            std::cout   << "X: "            << Selection[i]
                        << " | EDF_B: "     << EDFBefore
                        << " | EDF_A: "     << EDFAfter
                        << " | Theory: "    << ThDFVal << std::endl;
        }

        // Обновление максимальной дельты
        if (DiffBefore > Dmax) Dmax = DiffBefore;
        if (DiffAfter > Dmax)  Dmax = DiffAfter;
    }

    if (Debug) {
        std::cout << "Final Dmax: " << Dmax << std::endl;
    }

    // --- ![Формула поиска лямбды](../../Images/Comments/3.png)
    double lambda = Dmax * std::sqrt(selection_size);

    return lambda;
}