cmake_minimum_required(VERSION 3.14)
project(Project CXX)

# Устанавливаем стандарт C++17 (Matplot++ требует минимум C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем модуль FetchContent
include(FetchContent)

# Объявляем зависимость Matplot++
FetchContent_Declare(
        matplotplusplus
        GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
        GIT_TAG        v1.2.0 # Рекомендуется указать конкретную стабильную версию или тег
)

# Скачиваем и добавляем библиотеку в проект
FetchContent_MakeAvailable(matplotplusplus)

# Объявляем ваш исполняемый файл
add_executable(Project main.cpp
        Utils/View/View.cpp
        Utils/View/View.h
        TasksHandlers/Task1Handler/Task1Handler.cpp
        TasksHandlers/Task1Handler/Task1Handler.h
        TasksHandlers/Task1Handler/Task1Handler.h
        Utils/Generation/Generation.cpp
        Utils/Generation/Generation.h
        TasksHandlers/Task2Handler/Task2Handler.cpp
        TasksHandlers/Task2Handler/Task2Handler.h
        TasksHandlers/Task3Handler/Task3Handler.cpp
        TasksHandlers/Task3Handler/Task3Handler.h
        TasksHandlers/Task4Handler/Task4Handler.cpp
        TasksHandlers/Task4Handler/Task4Handler.h
        Utils/Math/Math.cpp
        Utils/Math/Math.h
        Utils/Converter/Converter.cpp
        Utils/Converter/Converter.h
        Utils/XLSView/XLSView.cpp
        Utils/XLSView/XLSView.h)

# Ищем библиотеку libxlsxwriter для работы с экселем
find_library(XLSXWRITER_LIB xlsxwriter REQUIRED)
find_path(XLSXWRITER_INCLUDE_DIR xlsxwriter.h REQUIRED)

# Передаем пути к заголовочным файлам xlsxwriter конкретно вашему проекту
target_include_directories(Project PRIVATE ${XLSXWRITER_INCLUDE_DIR})


# Связываем ваш проект со ВСЕМИ библиотеками
target_link_libraries(Project PRIVATE matplot ${XLSXWRITER_LIB})
