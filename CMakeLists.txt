cmake_minimum_required(VERSION 3.20)

# Имя проекта и версия
project(IheayProject VERSION 1.0 LANGUAGES CXX)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Включаем папку include/ для заголовочных файлов
include_directories(${PROJECT_SOURCE_DIR}/include)

# Собираем все cpp файлы из src/
file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

# Создаем исполняемый файл
add_executable(iheay_app ${SOURCES})

# Опции компилятора (необязательно, но полезно)
if(MSVC)
    target_compile_options(iheay_app PRIVATE /W4 /permissive-)
else()
    target_compile_options(iheay_app PRIVATE -Wall -Wextra -Wpedantic -O2)
endif()

# Установка (опционально)
install(TARGETS iheay_app DESTINATION bin)
