cmake_minimum_required(VERSION 3.22)

project(DAFELab VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "~/work/cpp_libraries/qt-everywhere-src-5.15.2/qtbase")

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(DAFELab
    mainwindow.ui
    mainwindow.cpp
    main.cpp
        logic/manager.cpp logic/manager.h logic/variable_data.cpp logic/variable_data.h logic/instrument.cpp logic/instrument.h)

target_link_libraries(DAFELab Qt5::Widgets)