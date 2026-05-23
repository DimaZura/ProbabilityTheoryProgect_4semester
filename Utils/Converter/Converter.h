//
// Created by mac on 17.05.2026.
//

#ifndef PROJECT_CONVERTER_H
#define PROJECT_CONVERTER_H
#include <vector>


class Converter {
    public:

    // Перевод выборки в эмпирическую функцию распределения
    static std::vector<double> ToEDF(std::vector <double> Selection, int min, int max, double SelectionSize, int CountSegments = 0);
};


#endif //PROJECT_CONVERTER_H
