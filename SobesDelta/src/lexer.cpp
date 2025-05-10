//
// Created by Богдан on 09.05.2025.
//

#include <cctype>
#include <sstream>

#include "lexer.h"
#include "Token.h"

std::vector<std::unique_ptr<Token>> Lexer::tokenize()
{
    std::vector<std::unique_ptr<Token>> tokens;
    while (pos < input.size())
    {
        // Пропуск пробелов
        if (isspace(input[pos]))
        {
            pos++;
            continue;
        }

        // Числа
        if (isdigit(input[pos]) || (input[pos] == '-' && (pos == 0 || is_operator_before(tokens))))
        {
            double num = parse_number();
            tokens.push_back(std::make_unique<NumberToken>(num));
            continue;
        }

        // Переменные и функции
        if (isalpha(input[pos])) {
            std::string id = parse_identifier();
            if (id == "sin" || id == "cos" || id == "!")
            {
                tokens.push_back(std::make_unique<FunctionToken>(id));
            }
            else
            {
                tokens.push_back(std::make_unique<VariableToken>(id));
            }
            continue;
        }

        // Операторы
        if (is_operator(input[pos]))
        {
            tokens.push_back(std::make_unique<OperatorToken>(input[pos]));
            pos++;
            continue;
        }

        // Скобки
        if (is_bracket(input[pos]))
        {
            bool is_open = (input[pos] == '(' || input[pos] == '[' || input[pos] == '{');
            tokens.push_back(std::make_unique<BracketToken>(input[pos], is_open));
            pos++;
            continue;
        }

        // Неизвестный символ
        throw std::runtime_error("Unknown symbol: " + std::string(1, input[pos]));
    }
    return tokens;
}

// Вспомогательные методы
double Lexer::parse_number()
{
    std::stringstream ss;
    bool has_decimal = false;
    if (input[pos] == '-')
    {
        ss << input[pos++];
    }
    while (pos < input.size() && (isdigit(input[pos]) || input[pos] == '.'))
    {
        if (input[pos] == '.')
        {
            if (has_decimal) break;
            has_decimal = true;
        }
        ss << input[pos++];
    }
    return std::stod(ss.str());
}

std::string Lexer::parse_identifier()
{
    std::stringstream ss;
    while (pos < input.size() && (isalnum(input[pos]) || input[pos] == '_'))
    {
        ss << input[pos++];
    }
    return ss.str();
}

bool Lexer::is_operator(const char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool Lexer::is_bracket(const char c)
{
    return c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}';
}

bool Lexer::is_operator_before(const std::vector<std::unique_ptr<Token>>& tokens)
{
    return !tokens.empty() && (tokens.back()->getType() == Token::TokenType::Operator ||
                               tokens.back()->getType() == Token::TokenType::Bracket);
}