cmake_minimum_required(VERSION 3.22)

project(DAFELab VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets PrintSupport REQUIRED)

add_executable(DAFELab
    mainwindow.ui
    mainwindow.cpp
    main.cpp logic/variable_data.cpp logic/manager.cpp models/datamodel.cpp logic/io.cpp logic/instrument.cpp logic/io.cpp qcustomplot.cpp logic/utils.hpp)

target_link_libraries(DAFELab Qt5::Widgets Qt5::PrintSupport)
