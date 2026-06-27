cmake_minimum_required(VERSION 3.22)
project(ColorPicker LANGUAGES CXX)
set(APP_VERSION "0.0.2")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Widgets)
if(UNIX AND NOT APPLE)
    find_package(Qt6 6.2 REQUIRED COMPONENTS DBus)
endif()

set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/ColorSlot.cpp
    src/ColorPicker.cpp
    src/PickerOverlay.cpp
    src/ComparisonPanel.cpp
)
if(UNIX AND NOT APPLE)
    list(APPEND SOURCES src/WaylandPortalPicker.cpp)
endif()

add_executable(color-picker ${SOURCES})

target_include_directories(color-picker PRIVATE src)

target_link_libraries(color-picker PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
if(UNIX AND NOT APPLE)
    target_link_libraries(color-picker PRIVATE Qt6::DBus)
    target_compile_definitions(color-picker PRIVATE HAVE_WAYLAND_PORTAL)
endif()

if(MSVC)
    target_compile_options(color-picker PRIVATE /W4)
else()
    target_compile_options(color-picker PRIVATE -Wall -Wextra)
endif()

install(TARGETS color-picker DESTINATION bin)
