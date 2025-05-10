//
// Created by Богдан on 09.05.2025.
//

#pragma once
#include <string>
#include <utility>


class Token {
public:
    enum class TokenType { Number, Variable, Operator, Function, Bracket };

    virtual ~Token() = default;
    [[nodiscard]] virtual TokenType getType() const = 0;
};

/*
 * NumberToken - число (из выражения)
 */
class NumberToken final : public Token {
    double value;
public:
    explicit NumberToken(const double _value) : value(_value) {}
    [[nodiscard]] TokenType getType() const override { return TokenType::Number; }
    [[nodiscard]] double getValue() const { return value; }
};

/*
 * VariableToken - имя константы (из выражения)
 */
class VariableToken final : public Token {
    const std::string name;
public:
    explicit VariableToken(std::string  _name) : name(std::move(_name)) {}
    [[nodiscard]] TokenType getType() const override { return TokenType::Variable; }
    [[nodiscard]] std::string getName() const { return name; }
};

/*
 * OperatorToken - оператор (из выражения)
 */
class OperatorToken final : public Token {
    char oper; // '+', '-', '*', '/', '^'
public:
    explicit OperatorToken(const char _oper) : oper(_oper) {}
    [[nodiscard]] TokenType getType() const override { return TokenType::Operator; }
    [[nodiscard]] char getOperator() const { return oper; }
};

/*
 * FunctionToken - функции (из выражения)
 */
class FunctionToken final : public Token {
    std::string funcName; // "sin", "cos", "!"
public:
    explicit FunctionToken(std::string  name) : funcName(std::move(name)) {}
    [[nodiscard]] TokenType getType() const override { return TokenType::Function; }
    [[nodiscard]] std::string getFunctionName() const { return funcName; }
};

/*
 * BracketToken - оператор (из выражения)
 */
class BracketToken final : public Token {
    char bracket; // '(', ')', '[', ']', '{', '}'
    bool isOpen; // true для открывающих скобок
public:
    BracketToken(const char _bracket, const bool _isOpen) : bracket(_bracket), isOpen(_isOpen) {}
    [[nodiscard]] TokenType getType() const override { return TokenType::Bracket; }
    [[nodiscard]] char getBracket() const { return bracket; }
    [[nodiscard]] bool isOpenBracket() const { return isOpen; }
};