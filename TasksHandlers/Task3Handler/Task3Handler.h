//
// Created by mac on 17.05.2026.
//

#ifndef PROJECT_TASK3HANDLER_H
#define PROJECT_TASK3HANDLER_H
#include <vector>


class Task3Handler {
public:

    static void Run();

protected:

    static constexpr bool Debug = true;

    static void Process(int SelectionSize);

    static double CalculatePearsonTest(std::vector<double> Selection, int n, double p);
};


#endif //PROJECT_TASK3HANDLER_H
