cmake_minimum_required(VERSION 3.16)

project(GeometryDraw VERSION 1.0.0 LANGUAGES CXX)

# Включить автоматическую обработку Qt (MOC, UIC, RCC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)

# Добавить исполняемый файл
add_executable(${PROJECT_NAME}
    source/main.cpp
    source/mainwindow.cpp
    source/mainwindow.h
    source/Geometrydraw.ui
    source/Geometrydraw.qrc
 "source/geometry.h" "source/geometry.cpp")

# Связать с Qt-библиотеками
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
)

if(WIN32) # Отключить запуск консоли
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()