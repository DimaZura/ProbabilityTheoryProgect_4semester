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
    Process(25, 0.05);
    Process(100, 0.1);
    Process(500, 0.2);
    Process(1000,0.4);
    // Process(10000000,0.4);

    // сохранение таблицы расчетных данных
    XLSView::SaveMxDxTable(AnalyticsMxDxResult, 2);

    // сохранение таблицы оценок
    XLSView::SaveTestResult(PearsonTests, 2);

}

void Task2Handler::Process(int SelectionSize, double Alpha) {

    //
    //  Начальные параметры
    //

    int n = 8;
    double p = 0.3;

    //
    //  Генерация выборки
    //

    std::vector<double> Selection = Generation::CreateASelectionByBinomialDistribution(SelectionSize, n, p);

    std::vector<double> EDF = Converter::ToEDF(Selection, 0, 8, SelectionSize);

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

    XLSView::SaveStatisticalSeriesForDiscrete(EDF, 0, 8, SelectionSize, 2);

    //
    //  Вычисление оценки χ2 Пирсона
    //

    // --- определение критического значения критерия ---

    std::map<double, double> AlphaToHiCrit {
                {0.01, 20.09}, // 15.09
                {0.05, 15.51},
                {0.1, 13.36},
                {0.2, 11.03},
                {0.4, 8.45}
    };
    double HiCritical = AlphaToHiCrit.at(Alpha);

    // вычисление критерия
    double TestValue = CalculatePearsonTest(Selection, n, p);
    if (Debug) {
        std::cout << "Pearson Test: " << TestValue << std::endl;
        std::cout << std::endl;
    }

    // сохранение

    TestResult TestResult(TestValue, HiCritical, Alpha);
    PearsonTests.TestsResults.insert({SelectionSize, TestResult});

}


double Task2Handler::CalculatePearsonTest(std::vector<double> Selection, int n, double p) {

//     //
//     //  Функции распределения
//     //
//
//     // ЭФР
//     std::vector<double> Empirical = Converter::ToEDF(Selection, 0, 8, Selection.size());
//     // ТФР
//     std::vector<double> Theory = Math::TheoryEDFBinomialDistribution(n, p);
//
//     //
//     //  Разбиение на сегменты с количеством элементов
//     //
//
//     size_t empSize = Empirical.size();
//     size_t thSize = Theory.size();
//
//     std::vector<int> NEmpirical(empSize);
//     std::vector<int> NTheory(thSize);
//
//     NEmpirical[0] = std::round((Empirical[0])*Selection.size());
//     for (int i = 1; i < empSize; i++) {
//         NEmpirical[i] = std::round((Empirical[i]-Empirical[i-1])*Selection.size());
//     }
//
//     NTheory[0] = std::round((Theory[0])*Selection.size());
//     for (int i = 1; i < thSize; i++) {
//         NTheory[i] = std::round((Theory[i]-Theory[i-1])*Selection.size());
//     }
//
//     if (Debug) {
//         std:: cout << "До склейки" << std::endl;
//         for (auto val : NEmpirical) {
//             std:: cout << val << " ";
//         }
//         std:: cout << std::endl;
//         for (auto val : NTheory) {
//             std:: cout << val << " ";
//         }
//         std:: cout << std::endl;
//     }
//
//     // поиск сегментов содержащих 0 элементов и склейка их с ненулевыми
//     // пример склейки двух разбиений
//     // [0, 1, 5, 10, 3, 1, 0]    --->    [6, 10, 3, 1]
//     // [0, 0, 2, 8, 19, 6, 1]    --->    [2, 8, 19, 7]
//     //  склеены первые три сегмента  и последние два
//     //
//     // (необходимо для корректной работы алгоритма вычисления оценки, чтобы не допускать деление на ноль)
//     Math::DivideIntoNonZeroSegments(NEmpirical, NTheory);
//
//     if (Debug) {
//         std:: cout << "После склейки" << std::endl;
//
//         for (auto val : NEmpirical) {
//             std:: cout << val << " ";
//         }
//         std:: cout << std::endl;
//         for (auto val : NTheory) {
//             std:: cout << val << " ";
//         }
//         std:: cout << std::endl;
//     }
//
//
//     // ![оценка хи2 Пирсона](../../Images/Comments/10.png)
//     // Пояснение об используемых переменных
//     // NEmpirical[i] = m[i] - число наблюдения случайной велечины на i-ом интервале
//     // NTheory[i] = n*p[i] - ожидаемое чило наблюдения СВ на i-ом интервале
//     // где p[i] - теоретическая вероятность появления СВ на i-ом интервале
//
//     double result = 0;
//     size_t nThSize = NTheory.size();
//
//     for (int i = 0; i < nThSize; i++) {
//
// //                                  (m[i]-n*p[i])**2                             / (n*p[i])
//         result += double(NEmpirical[i] - NTheory[i])*(NEmpirical[i] - NTheory[i])/NTheory[i];
//     }



    std::vector<int> m(9);
    double result = 0.0;
    int size = Selection.size();
    std::vector<double> EDFTh = Math::TheoryEDFBinomialDistribution(n, p);
    std::vector<double> pTh;

    pTh.push_back(EDFTh[0]);
    for (int i = 1; i<EDFTh.size(); i++) {
        pTh.push_back(EDFTh[i] - EDFTh[i-1]);
    }

    for (int val : Selection) {
        if (val >=0 && val<=n) {
            m[val] ++;
        }
    }

    for (int i = 0; i<n; i++) {
        result += (m[i]-size*pTh[i])*(m[i]-size*pTh[i])/(size*pTh[i]);
    }

    return result;
}