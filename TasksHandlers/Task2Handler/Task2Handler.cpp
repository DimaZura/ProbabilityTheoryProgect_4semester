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



double Task2Handler::ThMx = 0.0;
double Task2Handler::ThDx = 0.0;

std::map<int, double> Task2Handler::Dxs = std::map<int, double>{};
std::map<int, double> Task2Handler::Mxs = std::map<int, double>{};



void Task2Handler::Run() {
    Process(25);
    Process(100);
    Process(500);
    Process(1000);
    Process(10000000);

    XLSView::SaveMxDxTable(ThMx, ThDx, Mxs, Dxs, 2);
}

void Task2Handler::Process(int SelectionSize) {

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

    Mxs.insert({SelectionSize, EmpMx});
    Dxs.insert({SelectionSize, EmpDx});

    //  --- Теоретические значения ---


    ThMx = n*p;
    ThDx = n*p*(1-p);


    //
    //  Статистический ряд
    //

    std::vector<double> EDF = Converter::ToEDF(Selection, 0, 8, SelectionSize);

    XLSView::SaveStatisticalSeriesForDiscrete(EDF, 0, 8, SelectionSize, 2);


}
