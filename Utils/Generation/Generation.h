//
// Created by mac on 16.05.2026.
//

#ifndef PROJECT_GENERATION_H
#define PROJECT_GENERATION_H

#include <cmath>
#include <random>
#include <vector>

class Generation {

public:

    // Генерация выборки равномерного распределения
    static std::vector<double> CreateASelectionByRealDistribution(int size, double min, double max);

    // Генерация выборки биномиального распределения
    static std::vector<double> CreateASelectionByBinomialDistribution(int size, int n, double p);

protected:

    // Генератор чисел по бином распределению
    static int generate_binomial_distribution(int n, double p, std::mt19937& gen, std::uniform_real_distribution<double>& dist_01);
};


#endif //PROJECT_GENERATION_H
