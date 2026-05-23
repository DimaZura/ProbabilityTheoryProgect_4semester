//
// Created by mac on 20.05.2026.
//

#ifndef PROJECT_XLSVIEW_H
#define PROJECT_XLSVIEW_H
#include <map>


class XLSView {
public:

    // Сохранить таблицу сравнения математического ожидания и дисперсии
    static void SaveMxDxTable(double ThMx, double ThDx, std::map<int, double> Mxs, std::map<int, double> Dxs, int taskNum);

    // сохранить статистический ряд 
    static void SaveStatisticalSeriesForContinuous(std::vector<double> EDF, int min, int max, int SelectionSize, int taskNum);

    static void SaveStatisticalSeriesForDiscrete(std::vector<double> EDF, int min, int max, int SelectionSize, int taskNum);

};


#endif //PROJECT_XLSVIEW_H
