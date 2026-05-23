//
// Created by mac on 16.05.2026.
//

#include "Generation.h"

#include <iostream>
#include <ostream>
#include <random>


// Возвращает выборку длиной size равномерного распределения
//  min - минимальное значение
//  max - максимальное значение
std::vector<double> Generation::CreateASelectionByRealDistribution(int size, double min, double max) {
    std::vector<double> result;

    // инициализация объектов для генерации случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> d(min, max);

    for (int i = 0; i < size; i++) {
        double value = d(gen);
        result.push_back(value);
        // std::cout << value << " ";
    }
    // std::cout << std::endl;

    return result;
}

// Возвращает выборку длиной size биномиального распределения
//  n - число независимых испытаний
//  p - вероятность очередного успеха
std::vector<double> Generation::CreateASelectionByBinomialDistribution(int size, int n, double p) {
    std::vector<double> result;

    // инициализация объектов для генерации случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> d(0, 1);

    for (int i = 0; i < size; i++) {
        int value = generate_binomial_distribution(n, p, gen, d);


        result.push_back(value);
    }

    return result;
}

// возвращает число сгенерированное по биномиальному распределению
int Generation::generate_binomial_distribution(int n, double p, std::mt19937 &gen, std::uniform_real_distribution<double> &dist_01) {
    int success = 0;
    for (int i = 0; i < n; i++) {
        double rand = dist_01(gen);
        if (rand < p) {
            success++;
        }
    }

    return success;
}
