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


double Task1Handler::ThMx = 0.0;
double Task1Handler::ThDx = 0.0;

std::map<int, double> Task1Handler::Dxs = std::map<int, double>{};
std::map<int, double> Task1Handler::Mxs = std::map<int, double>{};


void Task1Handler::Run() {
    Process(25);
    Process(100);
    Process(500);
    Process(1000);
    Process(10000000);

    XLSView::SaveMxDxTable(ThMx, ThDx, Mxs, Dxs, 1);
}


void Task1Handler::Process(int SelectionSize) {
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
    //  Гистограмма распределения
    //

    View::SaveHist(Selection,min, max, CountSegments, 1, std::to_string(SelectionSize) + "Hist");

    //
    //  Эмпирическая функция распределения
    //

    View::SaveEmpiricalDistributionFunctionOfRealDistribution(Selection, max, min, CountSegments, 1, std::to_string(SelectionSize) + "EDF" );

    //
    //  Математическое ожидание и Дисперсия
    //

    // 3TODO доделать практические оценки !!(занести в табличку из конца методички)!!

    //  --- Эмпирические значения ---


    // ![Математическое ожидание](../../Images/Comments/8.png)
    double EmpMx = Math::CalculateExpectedValue(Selection);
    // ![Дисперсия](../../Images/Comments/9.png)
    double EmpDx = Math::CalculateVariance(Selection);


    Mxs.insert({SelectionSize, EmpMx});
    Dxs.insert({SelectionSize, EmpDx});


    //  --- Теоретические значения ---


    // ![Математическое ожидание](../../Images/Comments/6.png)
    ThMx = double(min+max)/2;
    // ![Дисперсия](../../Images/Comments/7.png)
    ThDx = std::pow(double(max-min), 2) / 12;


    //
    //  Статистический ряд
    //

    std::vector<double> EDF = Converter::ToEDF(Selection, min, max, SelectionSize, CountSegments);

    XLSView::SaveStatisticalSeriesForContinuous(EDF, min, max, SelectionSize, 1);

}