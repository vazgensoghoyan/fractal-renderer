# main CMakeLists.txt in repo root

cmake_minimum_required(VERSION 3.20)
project(IheayProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Если используешь git submodule для raylib
add_subdirectory(external/raylib)

find_package(OpenMP REQUIRED)

# Подключаем свои include
include_directories(${PROJECT_SOURCE_DIR}/include)

# Собираем библиотеку без main.cpp
file(GLOB_RECURSE LIB_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)
list(FILTER LIB_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

add_library(iheay_lib ${LIB_SOURCES})
target_link_libraries(iheay_lib PUBLIC OpenMP::OpenMP_CXX raylib)

# Собираем исполняемый файл
add_executable(iheay_app src/main.cpp)
target_link_libraries(iheay_app PRIVATE iheay_lib raylib)

# Компиляторные флаги
if(MSVC)
    target_compile_options(iheay_lib PRIVATE /W4 /permissive-)
else()
    target_compile_options(iheay_lib PRIVATE -Wall -Wextra -Wpedantic -O2)
endif()

# Тесты
enable_testing()
add_subdirectory(external/googletest)
add_subdirectory(tests)

# Примеры использования
add_subdirectory(usage_examples)
