//
// Created by mac on 17.05.2026.
//

#include "Task4Handler.h"

#include <iostream>
#include <ostream>
#include <vector>
#include <cmath>
#include <map>

#include "../../Utils/Converter/Converter.h"
#include "../../Utils/Generation/Generation.h"
#include "../../Utils/Math/Math.h"
#include "../../Utils/View/View.h"


void Task4Handler::Run() {
    Process(25, 0.2);
    Process(100,0.3);
    Process(500,0.4);
    Process(1000,0.5);
}

void Task4Handler::Process(int SelectionSize, double Alpha) {

    // --- определение критического значения критерия ---

    std::map<double, double> AlphaToLambdaCrit {
        {0.2, 1.07},
        {0.3, 0.97},
        {0.4, 0.89},
        {0.5, 0.83}
    };

    double LambdaCrit = AlphaToLambdaCrit.at(Alpha);

    // --- параметры распределения ---

    int min = 2;
    int max = 6;

    //
    //  Генерация выборки
    //

    std::vector<double> Selection = Generation::CreateASelectionByRealDistribution(SelectionSize, min, max);

    //
    //  Вычисление оценки близости гипотетического и эмпирического ряда по оценке λ-Колмагорова
    //

    // TODONE сравнить со значением 1.2 (строится по уровню значимости 0.112 (или больше 0.4 , 0.5) [вероятность отвергнуть правильную гипотезу])  .   Построить статистический ряд (тот что первый в конспекте)
    double  KolmogorovTest = CalculateKolmogorovTest(Selection, min, max);

    std::cout << "Test : " <<  KolmogorovTest << std::endl;

    // --- Запись в файл ---

    std::string fileName = "KolmogorovTest " + std::to_string(SelectionSize);
    View::SaveTest(KolmogorovTest, 4, fileName, LambdaCrit);
}

// Производит вычисление критерия Колмагорова
double Task4Handler::CalculateKolmogorovTest(std::vector<double> Selection, int min, int max) {

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


