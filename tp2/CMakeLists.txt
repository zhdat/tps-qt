cmake_minimum_required(VERSION 3.27)
project(tp2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.14.0\\5.14.0\\mingw73_32\\")

include_directories(Calculator)

find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(tp2
        Calculator/CalculatorModel.cpp
        Calculator/CalculatorModel.h
        Calculator/main.cpp
        Calculator/mainwindow.cpp
        Calculator/mainwindow.h)
target_link_libraries(tp2 Qt5::Widgets)