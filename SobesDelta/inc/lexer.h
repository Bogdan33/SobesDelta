//
// Created by Богдан on 09.05.2025.
//

#pragma once
#include <memory>       // Для std::unique_ptr
#include <string>
#include <utility>
#include <vector>
#include "Token.h"     // Подключение объявлений токенов

class Lexer {
public:
    // Конструктор: принимает входную строку для токенизации
    explicit Lexer(std::string  _input) : input(std::move(_input)) {};

    // Основной метод: возвращает вектор токенов
    std::vector<std::unique_ptr<Token>> tokenize();

private:
    // Приватные вспомогательные методы
    double parse_number();              // Парсинг чисел (например, "3.14")
    std::string parse_identifier();     // Парсинг идентификаторов (переменных/функций)
    [[nodiscard]] static bool is_operator(char c) ;     // Проверка, является ли символ оператором
    [[nodiscard]] static bool is_bracket(char c) ;      // Проверка, является ли символ скобкой
    [[nodiscard]] static bool is_operator_before(const std::vector<std::unique_ptr<Token>>& tokens) ;    // Проверка контекста для унарного минуса

    // Поля класса
    std::string input;  // Входная строка
    size_t pos = 0;     // Текущая позиция в строке
};
