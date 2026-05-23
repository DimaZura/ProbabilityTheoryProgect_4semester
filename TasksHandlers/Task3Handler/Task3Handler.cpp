//
// Created by mac on 17.05.2026.
//

#include "Task3Handler.h"

#include <vector>

#include "../../Utils/Converter/Converter.h"
#include "../../Utils/Generation/Generation.h"
#include "../../Utils/Math/Math.h"
#include "../../Utils/View/View.h"
#include "matplot/axes_objects/contours.h"
#include "matplot/util/common.h"


void Task3Handler::Run() {
    Process(25);
    Process(100);
    Process(500);
    Process(1000);
}

void Task3Handler::Process(int SelectionSize) {

    int n = 8;
    double p = 0.3;

    //
    //  Генерация выборки
    //

    std::vector<double> Selection = Generation::CreateASelectionByBinomialDistribution(SelectionSize, n, p);

    //
    //  Вычисление оценки близости гипотетического и эмпирического ряда по оценке X^2 Пирсона
    //

    double PearsonTest = CalculatePearsonTest(Selection, n, p);

    if (Debug) {
        std::cout << "Pearson Test: " << PearsonTest << std::endl;
        std::cout << std::endl;
    }

    std::string filename = "PearsonTest " + std::to_string(SelectionSize);
    View::SaveTest(PearsonTest, 3,  filename);
}

// Производит вычисление Критерия X^2 Пирсона
double Task3Handler::CalculatePearsonTest(std::vector<double> Selection, int n, double p) {

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