cmake_minimum_required(VERSION 3.22)
project(ColorPicker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Widgets DBus)

add_executable(color-picker
    src/main.cpp
    src/MainWindow.cpp
    src/ColorSlot.cpp
    src/ColorPicker.cpp
    src/PickerOverlay.cpp
    src/WaylandPortalPicker.cpp
    src/ComparisonPanel.cpp
)

target_include_directories(color-picker PRIVATE src)

target_link_libraries(color-picker PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::DBus
)

target_compile_options(color-picker PRIVATE -Wall -Wextra)
