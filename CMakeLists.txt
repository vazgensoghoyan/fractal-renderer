cmake_minimum_required(VERSION 3.10)
project(BMP_Project C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Пути к include
include_directories(${CMAKE_SOURCE_DIR}/include)

# Источники библиотеки
set(BMP_SOURCES
    src/bmp.c
    src/bmp_io.c
)

# Создаём статическую библиотеку bmp
add_library(bmp STATIC ${BMP_SOURCES})

# Исполняемый файл (main)
add_executable(bmp_demo src/main.c)

# Линкуем библиотеку к исполняемому файлу
target_link_libraries(bmp_demo PRIVATE bmp)
