//
// Created by mac on 15.05.2026.
//

#ifndef PROJECT_VIEW_H
#define PROJECT_VIEW_H
#include <vector>
#include <string>

class View {
public:
    // Генерация и сохранение гистограммы
    static void SaveHist(const std::vector<double>& Selection, int min, int max, int CountSegments, int taskNum, const std::string& fileName);

    // Генерация и сохранение полигона распределения
    static void SavePolygon(const std::vector<double>& values, int taskNum, const std::string& fileName);

    // Генерация и сохранение эмпирической функции равномерного распределения
    static void SaveEmpiricalDistributionFunctionOfRealDistribution(const std::vector<double>& Selection, int max, int min, int CountSegments, int taskNum, const std::string &fileName);

    // Генерация и сохранение эмпирической функции биномиального распределения
    static void SaveEmpiricalDistributionFunctionOfBinomialDistribution(const std::vector<double>& Selection, int n, double p, int taskNum, const std::string &fileName);

    // Производит сохранение оценки распределения
    static void SaveTest(double test, int taskNum, const std::string &fileName, double CriticalValue = 0.0);
};


#endif //PROJECT_VIEW_H
