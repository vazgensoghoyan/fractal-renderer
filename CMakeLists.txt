# main CMakeLists.txt in repo root

cmake_minimum_required(VERSION 3.20)
project(IheayProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenMP REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE LIB_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

list(FILTER LIB_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

add_library(iheay_lib ${LIB_SOURCES})

target_link_libraries(iheay_lib
    PUBLIC OpenMP::OpenMP_CXX
)

if(MSVC)
    target_compile_options(iheay_lib PRIVATE /W4 /permissive-)
else()
    target_compile_options(iheay_lib PRIVATE -Wall -Wextra -Wpedantic -O2)
endif()

add_executable(iheay_app src/main.cpp)
target_link_libraries(iheay_app iheay_lib)

enable_testing()
add_subdirectory(tests/googletest)
add_subdirectory(tests)

add_subdirectory(usage_examples)
