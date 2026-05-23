//
// Created by mac on 17.05.2026.
//

#include "Math.h"

#include <iostream>
#include <ostream>

// Функция для вычисления сочетаний C(n, k)
double Math::combinations(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    double res = 1.0;
    for (int i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

// Возвращает теоретическую эмпирическую функцию биномиального распределения
std::vector<double> Math::TheoryEDFBinomialDistribution(int n, double p) {
    std::vector<double> result;

    // общая вероятность
    double overall_probability = 0.0;
    for (int k = 0; k <= n; k++) {
        double probability = combinations(n, k) * std::pow(p, k) * std::pow(1.0 - p, n - k);
        overall_probability += probability;

        result.push_back(overall_probability);
    }

    return result;
}

// Возвращает теоретическую эмпирическую функцию равномерного распределения
double Math::TheoryRealDistribution(double x, int min, int max) {
    double result = 0.0;

    if (x <= min ) result = 0.0;
    else if (x >= max) result = 1.0;
    else {
        result = (x-min)/(max-min);
    }

    return result;
}

// Производит склейку нулевых отрезков с не нулевыми,
// размер векторов должен быть одинаков
void Math::DivideIntoNonZeroSegments(std::vector<int>& VecA, std::vector<int>& VecB) {
    if (VecA.size() != VecB.size()) {return;}
    std::vector<int> tempA;
    std::vector<int> tempB;

    int ASegmentValue = 0;
    int BSegmentValue = 0;
    for (int i = 0; i < VecA.size(); i++) {


        if (VecA[i] != 0 && VecB[i] != 0) {
            ASegmentValue += VecA[i];
            BSegmentValue += VecB[i];
        }
        else {
            ASegmentValue += VecA[i];
            BSegmentValue += VecB[i];


            if (i == VecA.size()-1) {
                tempA[tempA.size()-1] += ASegmentValue;
                tempB[tempB.size()-1] += BSegmentValue;
            }

            continue;
        }


        tempA.push_back(ASegmentValue);
        tempB.push_back(BSegmentValue);

        ASegmentValue = 0;
        BSegmentValue = 0;
    }

    VecA = tempA;
    VecB = tempB;
}

// Возвращает значение математического ожидания выборки Selection
double Math::CalculateExpectedValue(const std::vector<double> &Selection) {
    double result = 0.0;

    for (double val : Selection) {
        result += val;
    }

    result /= Selection.size();

    return result;
}

// Возвращает значение дисперсии выборки Selection
double Math::CalculateVariance(const std::vector<double> &Selection) {
    double result = 0.0;

    double ExpectedValue = CalculateExpectedValue(Selection);

    for (double val : Selection) {
        result += (val - ExpectedValue)*(val - ExpectedValue);
    }

    result /= double(Selection.size()-1);

    return result;
}
