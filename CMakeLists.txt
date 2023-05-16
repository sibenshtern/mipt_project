cmake_minimum_required(VERSION 3.20)

project(DAFELab VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets PrintSupport REQUIRED)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/qcustomplot.h ${CMAKE_CURRENT_BINARY_DIR}/DAFELab_autogen/include/qcustomplot.h COPYONLY)

add_executable(DAFELab
        mainwindow.ui
        mainwindow.cpp

        main.cpp

        logic/instrument.cpp
        logic/variable_data.cpp
        logic/manager.cpp
        logic/io_csv.cpp
        logic/blocks.h
        logic/odf.cpp

        models/datamodel.cpp
        models/visualmodel.cpp
        models/namingmodel.cpp
        models/instrumentmodel.cpp

        comboboxdelegate.cpp
        colordelegate.cpp

        qcustomplot.cpp
        plots/plotscatter.cpp
        plots/plothist.cpp
        plots/plotchoice.cpp
        plots/plot.h

        parser/FormulaParser.cpp
        logic/blocks.cpp logic/io_json.cpp)

target_link_libraries(DAFELab Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport /usr/lib/x86_64-linux-gnu/libboost_filesystem.a)
