//
// Created by mac on 17.05.2026.
//

#ifndef PROJECT_TASK4HANDLER_H
#define PROJECT_TASK4HANDLER_H
#include <vector>


class Task4Handler {

public:

    static void Run();

protected:

    static  constexpr bool Debug = false;

    static void Process(int SelectionSize, double Alpha);

    static double CalculateKolmogorovTest(std::vector<double> Selection, int min, int max);
};


#endif //PROJECT_TASK4HANDLER_H
