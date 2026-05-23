//
// Created by mac on 17.05.2026.
//

#include "Converter.h"

#include <iostream>
#include <ostream>


// перевод выборки в эмпирическую функцию распределения
//  min - минимальное значение выборки
//  max = максимальное значение выборки
//  SelectionSize - размер выборки
//  CountSegments [не обязательный параметр] - число сегментов
std::vector<double> Converter::ToEDF(std::vector<double> Selection, int min, int max, double SelectionSize, int CountSegments) {
    // установка числа мегментов
    // если CountSegments не задано то устанавливает сегменты длиной 1
    // если CountSegments !=0 (то есть указано при вызове) то производит вычисление числа сегментов
    int resultSize = max - min+1;
    if (CountSegments==0) {
        CountSegments = max-min;
    }
    else {
        resultSize = CountSegments;
    }

    std::vector<double> result;
    result.reserve(resultSize);

    // Подсчет сколько раз встречается каждое число.
    // Происхводит преобразования -
    //  1. смещение сегмента на отрезок [0, max-min]
    //  2. определение индекса с учетом числа сегментов
    std::vector<int> counts(resultSize, 0);
    for (double val : Selection) {
        val -= min;
        val *= double(CountSegments)/(max-min);
        int intVal = static_cast<int>(val);
        counts[intVal]++;
    }

    // Подсчет итогового результата
    double accumulated_count = 0;
    for (int i = 0; i < counts.size(); i++) {
        accumulated_count += counts[i]; // Включает текущее значение (аналог <=)
        result.push_back(accumulated_count / SelectionSize);
    }

    return result;
}

