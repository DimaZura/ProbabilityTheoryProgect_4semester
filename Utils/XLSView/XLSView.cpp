//
// Created by mac on 20.05.2026.
//

#include "XLSView.h"

#include <iomanip>
#include <iostream>
#include <map>
#include <ostream>
#include <strstream>
#include <xlsxwriter.h>
#include <vector>


// Сохранение таблицы мат ожидания и дисперсии
void XLSView::SaveMxDxTable(AnalyticsMxDxResult MxDxResult, int taskNum) {

    //
    //  Параметры
    //

    int ResultSize = MxDxResult.Emp.size();
    double ThMx = MxDxResult.Th.Mx;
    double ThDx = MxDxResult.Th.Dx;

    //
    // Создание объекта таблицы
    // (сохранение производится в папку image)
    //

    std::string FileName = "../Images/" + std::to_string(taskNum) + "/Analytics.xlsx";
    lxw_workbook  *workbook  = workbook_new(FileName.c_str());
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    //
    //  Создание стилей
    //

    lxw_format *str_format = workbook_add_format(workbook);
    format_set_align(str_format, LXW_ALIGN_CENTER);
    format_set_align(str_format, LXW_ALIGN_VERTICAL_CENTER);
    format_set_bold(str_format);
    format_set_border(str_format, LXW_BORDER_THIN);

    lxw_format *data_format = workbook_add_format(workbook);
    format_set_border(data_format, LXW_BORDER_THIN);

    //
    //  Объединение ячеек
    //

    worksheet_merge_range(worksheet, 0, 0, 2, 0, "Объем выборки n", str_format);
    worksheet_merge_range(worksheet, 0, 1, 2, 1, "Математическое ожидание Mx", str_format);
    worksheet_merge_range(worksheet, 0, 2, 2, 2, "Оценка математического ожидания Mx", str_format);
    worksheet_merge_range(worksheet, 0, 3, 2, 3, "% расхождения", str_format);
    worksheet_merge_range(worksheet, 0, 4, 2, 4,"Дисперсия Dx", str_format);
    worksheet_merge_range(worksheet, 0, 5, 2, 5,"Оценка дисперсии Dx", str_format);
    worksheet_merge_range(worksheet, 0, 6, 2, 6, "% расхождения", str_format);

    std::string str_ThMx = std::to_string(ThMx); // Сохраняем объект строки
    const char* char_ptr_ThMx = str_ThMx.c_str(); // Теперь этот char* будет валиден
    worksheet_merge_range(worksheet, 3, 1, 3 + ResultSize - 1, 1, char_ptr_ThMx, str_format);

    std::string str_ThDx = std::to_string(ThDx); // Сохраняем объект строки
    const char* char_ptr_ThDx = str_ThDx.c_str(); // Теперь этот char* будет валиден
    worksheet_merge_range(worksheet, 3, 4, 3 + ResultSize - 1, 4, char_ptr_ThDx, str_format);

    //
    //  Заполнение данных
    //

    int current_row = 3;
    for (auto pair : MxDxResult.Emp) {
        // --- Заполнение столбца объема выборки ---
        worksheet_write_number(worksheet, current_row, 0, pair.first, data_format);

        double Mx = pair.second.Mx;
        double Dx = pair.second.Dx;

        // --- Заполнение столбца мат ожидания ---
        worksheet_write_number(worksheet, current_row, 2, Mx, data_format);

        // --- Заполнение столбца оценки мат ожидания ---
        double test = std::abs(Mx - ThMx)/ThMx;
        worksheet_write_number(worksheet, current_row, 3, test, data_format);

        // --- Заполнение столбца дисперсии ---
        worksheet_write_number(worksheet, current_row, 5, Dx, data_format);

        // --- Заполнение столбца оценки дисперсии ---
        test = std::abs(Dx - ThDx)/ThDx;
        worksheet_write_number(worksheet, current_row, 6, test, data_format);

        current_row++;
    }

    // Сохранение объекта
    workbook_close(workbook);
}

// Сохраняет статистический ряд для непрерывной случайной величины
void XLSView::SaveStatisticalSeriesForContinuous(std::vector<double> EDF, int min, int max, int SelectionSize, int taskNum) {

    //
    // Создание объекта таблицы
    //

    std::string FileName = "../Images/" + std::to_string(taskNum) + "/" + std::to_string(SelectionSize) + "StatisticalSeries.xlsx";
    lxw_workbook  *workbook  = workbook_new(FileName.c_str());
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    //
    //  Заполнение верхней строки отрезков
    //


    double step = double(max-min)/EDF.size();
    for (int i = 0; i < EDF.size(); i++) {
        double Left = min + step*i;
        double Right = min + step*(i+1);
        std::string segment_str = "[ " + std::to_string(Left) + ",\n " + std::to_string(Right) + " )";
        if (i == EDF.size()-1) {
            segment_str = "[ " + std::to_string(Left) + ",\n " + std::to_string(Right) + " ]";
        }
        worksheet_write_string(worksheet, 0, i, segment_str.c_str(), nullptr);
    }

    //
    //  Заполнение строк данных
    //

    int m = EDF[0]*SelectionSize;
    double p = EDF[0];
    worksheet_write_number(worksheet, 1, 0 , m, nullptr);
    worksheet_write_number(worksheet, 2, 0,  p, nullptr);

    for (int i = 1; i < EDF.size(); i++) {
        std::cout << EDF[i] << " " << EDF[i-1] << std::endl;
        // --- Число элементов отрезка ---
        m = std::round((EDF[i]-EDF[i-1])*SelectionSize);
        worksheet_write_number(worksheet, 1, i , m, nullptr);

        // --- Вероятность попадания ---
        p = (EDF[i]-EDF[i-1]);
        worksheet_write_number(worksheet, 2, i, p, nullptr);
    }

    workbook_close(workbook);
}

void XLSView::SaveStatisticalSeriesForDiscrete(std::vector<double> EDF, int min, int max, int SelectionSize, int taskNum) {

    //
    // Создание объекта таблицы
    //

    std::string FileName = "../Images/" + std::to_string(taskNum) + "/" + std::to_string(SelectionSize) + "StatisticalSeries.xlsx";
    lxw_workbook  *workbook  = workbook_new(FileName.c_str());
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    //
    //  Заполнение верхней строки отрезков
    //


    for (int i = 0; i < EDF.size(); i++) {
        std::string segment_num = std::to_string(min + i);
        worksheet_write_string(worksheet, 0, i, segment_num.c_str(), nullptr);
    }

    //
    //  Заполнение строк данных
    //

    int m = EDF[0]*SelectionSize;
    double p = EDF[0];
    worksheet_write_number(worksheet, 1, 0 , m, nullptr);
    worksheet_write_number(worksheet, 2, 0,  p, nullptr);

    for (int i = 1; i < EDF.size(); i++) {
        // std::cout << EDF[i] << " " << EDF[i-1] << std::endl;
        // --- Число элементов отрезка ---
        m = std::round((EDF[i]-EDF[i-1])*SelectionSize);
        // std::cout << m << std::endl;
        worksheet_write_number(worksheet, 1, i , m, nullptr);

        // --- Вероятность попадания ---
        p = (EDF[i]-EDF[i-1]);
        worksheet_write_number(worksheet, 2, i, p, nullptr);
    }

    workbook_close(workbook);
}

void XLSView::SaveTestResult(AnalyticsTestsResult Result, int taskNum) {

    //
    // Создание объекта таблицы
    //

    std::string FileName = "../Images/" + std::to_string(taskNum) + "/TestAnalytics.xlsx";
    lxw_workbook  *workbook  = workbook_new(FileName.c_str());
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, 0, 0, "Размер выборки", nullptr);
    worksheet_write_string(worksheet, 0, 1, "Полученная оценка", nullptr);
    worksheet_write_string(worksheet, 0, 2, "Критическое значение", nullptr);
    worksheet_write_string(worksheet, 0, 3, "Значение альфа", nullptr);


    int i = 1;
    for (auto pair : Result.TestsResults) {
        auto Test = pair.second;
        worksheet_write_number(worksheet, i, 0, pair.first, nullptr);
        worksheet_write_number(worksheet, i, 1, Test.TestValue, nullptr);
        worksheet_write_number(worksheet, i, 2, Test.CriticalValue, nullptr);
        worksheet_write_number(worksheet, i, 3, Test.Alpha, nullptr);
        i++;
    }


    workbook_close(workbook);
}