//
// Created by mac on 17.05.2026.
//

#ifndef PROJECT_MATH_H
#define PROJECT_MATH_H
#include <vector>



class Math {
    public:

    // метод C(c, k)
    static double combinations(int n, int k);

    // Возвращает теоретическую эмпирическую функцию биномиального распределения
    static std::vector<double> TheoryEDFBinomialDistribution(int n, double p);

    // Возвращает теоретическую эмпирическую функцию равномерного распределения
    static double TheoryRealDistribution(double x, int min, int max);

    // Производит склейку нулевых отрезков с не нулевыми
    static void DivideIntoNonZeroSegments(std::vector<int>& VecA, std::vector<int>& VecB);

    // Вычисление математического ожидания
    static double CalculateExpectedValue(const std::vector<double>& Selection);

    // Вычисление дисперсии
    static double CalculateVariance(const std::vector<double>& Selection);
};


#endif //PROJECT_MATH_H
