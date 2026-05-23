//
// Created by mac on 16.05.2026.
//

#include "Task2Handler.h"

#include <iostream>
#include <ostream>
#include <vector>

#include "../../Utils/Converter/Converter.h"
#include "../../Utils/Generation/Generation.h"
#include "../../Utils/Math/Math.h"
#include "../../Utils/View/View.h"
#include "../../Utils/XLSView/XLSView.h"



AnalyticsMxDxResult Task2Handler::AnalyticsMxDxResult;
AnalyticsTestsResult Task2Handler::PearsonTests;


void Task2Handler::Run() {
    Process(25);
    Process(100);
    Process(500);
    Process(1000);
    Process(10000000);

    // сохранение таблицы расчетных данных
    XLSView::SaveMxDxTable(AnalyticsMxDxResult, 2);

    // сохранение таблицы оценок
    XLSView::SaveTestResult(PearsonTests, 2);

}

void Task2Handler::Process(int SelectionSize) {

    //
    //  Начальные параметры
    //

    int n = 8;
    double p = 0.3;

    //
    //  Генерация выборки
    //

    std::vector<double> Selection = Generation::CreateASelectionByBinomialDistribution(SelectionSize, n, p);


    //
    //  Полигон распределения
    //

    View::SavePolygon(Selection, 2, std::to_string(SelectionSize) + "Polygon");

    //
    //  Эмпирическая функция распределения
    //

    View::SaveEmpiricalDistributionFunctionOfBinomialDistribution(Selection, n, p, 2, std::to_string(SelectionSize) + "EmpiricalDistribution");

    //
    //  Математическое ожидание и Дисперсия
    //

    //  --- Эмпирические значения ---

    // ![Математическое ожидание](../../Images/Comments/8.png)
    double EmpMx = Math::CalculateExpectedValue(Selection);
    // ![Дисперсия](../../Images/Comments/9.png)
    double EmpDx = Math::CalculateVariance(Selection);

    MxDxResult EmpMxDxResult(EmpMx, EmpDx);
    AnalyticsMxDxResult.Emp.insert({SelectionSize, EmpMxDxResult});

    //  --- Теоретические значения ---

    AnalyticsMxDxResult.Th.Mx = n*p;
    AnalyticsMxDxResult.Th.Dx = n*p*(1-p);

    //
    //  Статистический ряд
    //

    std::vector<double> EDF = Converter::ToEDF(Selection, 0, 8, SelectionSize);

    XLSView::SaveStatisticalSeriesForDiscrete(EDF, 0, 8, SelectionSize, 2);

    //
    //  Вычисление оценки χ2 Пирсона
    //

    double TestValue = CalculatePearsonTest(Selection, n, p);
    if (Debug) {
        std::cout << "Pearson Test: " << TestValue << std::endl;
        std::cout << std::endl;
    }

    // сохранение

    TestResult TestResult(TestValue, 0.0, 0.0);
    PearsonTests.TestsResults.insert({SelectionSize, TestResult});

}


double Task2Handler::CalculatePearsonTest(std::vector<double> Selection, int n, double p) {

    //
    //  Функции распределения
    //

    // эмпирическая
    std::vector<double> Empirical = Converter::ToEDF(Selection, 0, 8, Selection.size());
    // теоретическая
    std::vector<double> Theory = Math::TheoryEDFBinomialDistribution(n, p);

    //
    //  Перевод в векторы содержащие
    //  число элеменов на нектором числе сегментов
    //  (дельты соседних значения вероятностей эмпирической или же теоретической ФР умноженные на размер выборки)
    //

    size_t empSize = Empirical.size();
    size_t thSize = Theory.size();

    std::vector<int> NEmpirical(empSize);
    std::vector<int> NTheory(thSize);

    NEmpirical[0] = double(Empirical[0])*Selection.size();
    for (int i = 1; i < empSize; i++) {
        NEmpirical[i] = double(Empirical[i]-Empirical[i-1])*Selection.size();
    }

    NTheory[0] = double(Theory[0])*Selection.size();
    for (int i = 1; i < thSize; i++) {
        NTheory[i] = double(Theory[i]-Theory[i-1])*Selection.size();
    }

    // поиск сегметов содержащих 0 элементов и склейка их с ненудевыми
    // пример склейки двух разбиений
    // [0, 1, 5, 10, 3, 1, 0]    --->    [6, 10, 3, 1]
    // [0, 0, 2, 8, 19, 6, 1]    --->    [2, 8, 19, 7]
    //  склеены первые три сегмента  и последние два
    //
    // (необходимо для корректрной работы алгоритма вычисления оценки, чтобе не допускать деление на ноль)
    Math::DivideIntoNonZeroSegments(NEmpirical, NTheory);

    if (Debug) {
        for (auto val : NEmpirical) {
            std:: cout << val << " ";
        }
        std:: cout << std::endl;
        for (auto val : NTheory) {
            std:: cout << val << " ";
        }
        std:: cout << std::endl;
    }


    //
    //  Вычисление: X^2 = Sum[i=0; i<n]((NEmp(i) - NTh(i))^2/NTh(i))
    //          NTh -   теоретическое число элементов попавших в i-й сегмент
    //          NEmp -  практическое число элементов попавших в i-й сегмент
    //

    double result = 0;
    size_t nThSize = NTheory.size();

    for (int i = 0; i < nThSize; i++) {
        result += (double(NTheory[i]) - double(NEmpirical[i]))*(NTheory[i] - NEmpirical[i])/NTheory[i];
    }


    return result;
}