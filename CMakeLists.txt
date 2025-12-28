cmake_minimum_required(VERSION 3.20)
project(code_visualizer C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключаем tree-sitter
add_subdirectory(external/tree-sitter)
add_subdirectory(external/QSchematic)

add_library(tree_sitter_cpp STATIC
    external/tree-sitter-cpp/src/parser.c
    external/tree-sitter-cpp/src/scanner.c
)
target_include_directories(tree_sitter_cpp PUBLIC external/tree-sitter-cpp/src)
target_link_libraries(tree_sitter_cpp tree-sitter)

# Исполняемый файл
add_executable(code_visualizer
    src/main.cpp
    src/dataContainers.cpp
    src/parser.cpp
)


find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Svg)

qt_standard_project_setup()

# Include пути
target_include_directories(code_visualizer PRIVATE
   ${CMAKE_SOURCE_DIR}/external/QSchematic/qschematic
   ${CMAKE_SOURCE_DIR}/external/QSchematic
)

# Линковка
target_link_libraries(code_visualizer PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Svg
    tree-sitter
    tree_sitter_cpp
    qschematic-static
)

set_target_properties(qschematic-static PROPERTIES CXX_STANDARD 23)