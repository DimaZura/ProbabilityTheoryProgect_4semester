//
// Created by mac on 16.05.2026.
//

#ifndef PROJECT_TASK1HANDLER_H
#define PROJECT_TASK1HANDLER_H
#include <map>


class Task1Handler {

public:

    static void Run();

protected:

    //
    //  Данные математического ожидания и дисперсии
    //

    static double ThMx;
    static double ThDx;

    static std::map<int, double> Mxs;
    static std::map<int, double> Dxs;

    //
    //  Выполнение задания
    //

    static void Process(int SelectionSize);

};


#endif //PROJECT_TASK1HANDLER_H
