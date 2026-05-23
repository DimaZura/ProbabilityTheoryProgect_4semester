//
// Created by mac on 15.05.2026.
//

#include "View.h"
#include <string>
#include <matplot/matplot.h>
#include <sys/stat.h>
#include <fstream>

#include "../Converter/Converter.h"
#include "../Math/Math.h"


void View::SaveHist(const std::vector<double>& Selection, int min, int max, int CountSegments, int taskNum, const std::string& fileName) {
    using namespace matplot;

    std::string fDir = "../Images/" + std::to_string(taskNum) + "/" + fileName + ".png";

    double step = double(max-min)/CountSegments;
    std::vector<double> edges;
    for (int i = 0; i < CountSegments+1; ++i) {
        double x_val = min + i * step;
        edges.push_back(x_val);
    }

    cla();
    hist(Selection, edges);

    // --- Оформление ---
    std::string strTitle = "Равномерное распределение: Гистограмма распределения " + std::to_string(Selection.size()) + " эл-ов";
    title(strTitle);
    xlabel("");
    ylabel("");
    xlim({double(min), double(max)});

    save(fDir);
}

void View::SavePolygon(const std::vector<double> &Selection, int taskNum, const std::string &fileName) {
    using namespace matplot;

    std::string fDir = "../Images/" + std::to_string(taskNum) + "/" + fileName + ".png";

    std::vector<double> Values(9);
    std::vector<double> x_val;

    for (int val : Selection) {
        Values[val] ++;
    }

    for (int i = 0; i < Values.size(); ++i) {
        x_val.push_back(i);
    }


    cla();
    stem(x_val, Values);

    // --- Оформление ---
    std::string strTitle = "Биномиальное распределение: Полигон распределения " + std::to_string(Selection.size()) + " эл-ов";
    title(strTitle);
    xlabel("");
    ylabel("");


    save(fDir);

}



void View::SaveEmpiricalDistributionFunctionOfRealDistribution(const std::vector<double>& Selection, int max, int min, int CountSegments, int taskNum, const std::string &fileName) {
    using namespace matplot;

    std::string fDir = "../Images/" + std::to_string(taskNum) + "/" + fileName + ".png";

    //
    //  Расчет эмпирической функции распределения
    //

    std::vector<double> EPF = Converter::ToEDF(Selection, min, max, Selection.size(), CountSegments);

    //
    //  Отрисовка
    //

    cla();          // очистка от предыдущих диаграмм
    hold(on);       // включение режима наслоения

    // --- Отрисовка теоретической функции ---

    std::vector<double> x_theory = {double(min) , double(max)};
    std::vector<double> y_theory = {0.0, 1.0};
    auto line = plot(x_theory, y_theory);
    line->line_width(3);
    line->color({0.0, 0.0, 0.46});

    // --- Отрисовка эмпирической функции (кусочно разрывная прямая)


    double step = double(max-min)/CountSegments;

    std::vector<double> x_val(CountSegments);
    std::vector<double> y_val(CountSegments);


    std::vector<double> stairs_x;
    std::vector<double> stairs_y;

    stairs_x.push_back(-0.5);
    stairs_y.push_back(0);
    for (int i = 0; i < EPF.size(); ++i) {
        double x_start = i * step + min;
        double y_val = EPF[i];

        std::cout << x_start << " " << x_start + step << " " << y_val << std::endl;

        // Запоминаем левую границу интервала и высоту ступени на нем
        stairs_x.push_back(x_start);
        stairs_y.push_back(y_val);
    }

    // Чтобы лесенка красиво завершилась в самом конце,
    // добавляем финальную правую границу последнего интервала
    double last_x_end = (EPF.size() - 1) * step + min + step; // или просто max
    stairs_x.push_back(last_x_end);
    stairs_y.push_back(EPF.back()); // сохраняем последнюю высоту

    // Отрисовываем идеальную ступенчатую линию одним вызовом
    auto stair_handle = stairs(stairs_x, stairs_y);
    stair_handle->line_width(4);

    // --- Оформление ---
    std::string strTitle = "Равномерное распределение: Теоретическая / Эмпирическая функции " + std::to_string(Selection.size()) + " эл-ов";
    title(strTitle);
    xlabel("Случайная величина (X)");
    ylabel("F(x)");
    xlim({double(min), double(max)});
    ylim({0, 1.05});
    grid(on);       // включение сетки

    save(fDir);

}

void View::SaveEmpiricalDistributionFunctionOfBinomialDistribution(const std::vector<double> &Selection, int n, double p, int taskNum, const std::string &fileName) {
    using namespace matplot;

    std::string fDir = "../Images/" + std::to_string(taskNum) + "/" + fileName + ".png";

    //
    //  Расчет эмпирической функции распределения
    //

    std::vector<double> emp_y = Converter::ToEDF(Selection, 0, 8, Selection.size(), 9);

    std::vector<double> emp_x(emp_y.size());
    for (int i = 0; i < emp_y.size(); ++i) {
        emp_x[i] = i;
    }

    //
    //  Расчет теоретической функции распределения
    //

    std::vector<double> theo_y = Math::TheoryEDFBinomialDistribution(n, p);

    std::vector<double> theo_x(theo_y.size());
    for (int i = 0; i < theo_y.size(); ++i) {
        theo_x[i] = i;
    }

    //
    //  Отрисовка
    //

    cla();          // очистка от предыдущих диаграмм
    hold(on);       // включение режима наслоения

    // --- Отрисовка теоретической функции ---

    auto theo_plot = stairs(theo_x, theo_y);
    theo_plot->line_width(2);
    theo_plot->color("rgb(12, 45, 87)"); // Тёмно-синий "идеал"

    // --- Отрисовка эмпирической функции (кусочно разрывная прямая)

    for (size_t i = 0; i < emp_x.size(); ++i) {
        double x_start = emp_x[i];
        double x_end = x_start + 1.0;
        double y_val = emp_y[i];

        auto stripe = plot(std::vector<double>{x_start, x_end}, std::vector<double>{y_val, y_val});
        stripe->line_width(4);
        stripe->color("blue");
    }

    // --- Оформление ---
    std::string strTitle = "Биномиальное распределение: Теоретическая / Эмпирическая функции " + std::to_string(Selection.size()) + " эл-ов";

    title(strTitle);
    xlabel("Количество успехов (X)");
    ylabel("F(x)");
    xlim({0, static_cast<double>(n) + 1});
    ylim({0, 1.05});
    grid(on);       // включение сетки

    save(fDir);
}


void View::SaveTest(double test, int taskNum, const std::string &fileName, double CriticalValue) {
    std::string fDir = "../Images/" + std::to_string(taskNum) + "/" + fileName + ".txt";

    std::ofstream FOutput = std::ofstream(fDir);

    FOutput << "Test = " << std::to_string(test) << std::endl;
    if (CriticalValue > 0) {
        FOutput << "CriticalValue = " << std::to_string(CriticalValue) << std::endl;
    }

    FOutput.close();
}
