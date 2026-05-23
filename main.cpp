#include <matplot/matplot.h>
#include "TasksHandlers/Task1Handler/Task1Handler.h"
#include "TasksHandlers/Task2Handler/Task2Handler.h"
#include "TasksHandlers/Task3Handler/Task3Handler.h"
#include "TasksHandlers/Task4Handler/Task4Handler.h"
#include "Utils/XLSView/XLSView.h"

int main() {

    // TODO ERROR в методичке есть ошибка - Таблица 1 последняя строка, там должно быть Pj

    Task1Handler::Run();
    // Task2Handler::Run();

    return 0;
}
