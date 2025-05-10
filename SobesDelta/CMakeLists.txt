cmake_minimum_required(VERSION 3.31)
project(SobesDelta)

set(CMAKE_CXX_STANDARD 20)

include_directories(lib)
include_directories(inc)

# Добавление исходников для библиотеки
add_library(calculator_lib
        src/Token.cpp
        src/lexer.cpp
        src/parser.cpp
        src/evaluator.cpp
)

add_executable(SobesDelta main.cpp)

# Подключение библиотеки к исполняемому файлу
target_link_libraries(SobesDelta calculator_lib)