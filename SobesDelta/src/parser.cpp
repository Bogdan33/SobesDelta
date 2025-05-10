//
// Created by Богдан on 10.05.2025.
//

#include "parser.h"

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <unordered_map>

Parser::Parser(std::vector<std::unique_ptr<Token>> tokens)
    : tokens_(std::move(tokens)) {}

std::vector<std::unique_ptr<Token>> Parser::parse() {
    while (pos < tokens_.size()) {
        // Перемещаем unique_ptr из вектора tokens_ во временную переменную
        std::unique_ptr<Token> token = std::move(tokens_[pos]);
        switch (token->getType()) {
            case Token::TokenType::Number:
            case Token::TokenType::Variable:
                output.push_back(std::move(token)); // Перемещаем во output
                break;
            case Token::TokenType::Function:
                handleFunction(std::move(token)); // Передаем владение
                break;
            case Token::TokenType::Bracket: {
                auto* bracket = static_cast<BracketToken*>(token.get());
                if (bracket->isOpenBracket()) {
                    stack.push(std::move(token)); // Перемещаем в стек
                } else {
                    handleBracket(std::move(token)); // Передаем владение
                }
                break;
            }
            case Token::TokenType::Operator:
                handleOperator(std::move(token)); // Передаем владение
                break;
            default:
                throw std::runtime_error("Unknown token type");
        }
        pos++; // Переходим к следующему токену
    }

    // Переместить оставшиеся операторы из стека
     while (!stack.empty()) {
         if (stack.top()->getType() == Token::TokenType::Bracket) {
             throw std::runtime_error("Mismatched brackets");
         }
         output.push_back(std::move(stack.top()));
         stack.pop();
    }

    return std::move(output);
}

// Обработка оператора
void Parser::handleOperator(std::unique_ptr<Token> op) {
    while (!stack.empty()) {
        auto& top = stack.top();
        if (top->getType() != Token::TokenType::Operator &&
            top->getType() != Token::TokenType::Function) break;

        const int curr_priority = getPriority(*op);
        const int top_priority = getPriority(*top);

        if (const bool right_assoc = isRightAssociative(*op); (right_assoc && curr_priority < top_priority) ||
                                                        (!right_assoc && curr_priority <= top_priority)) {
            output.push_back(std::move(top));
            stack.pop();
            } else {
                break;
            }
    }
    stack.push(std::move(op)); // Перемещаем op в стек
}

// Обработка функций
void Parser::handleFunction(std::unique_ptr<Token> func) {
    stack.push(std::move(func)); // Передаем владение в стек
}

// Обработка скобок
void Parser::handleBracket(const std::unique_ptr<Token> bracket) {
    if (bracket->getType() != Token::TokenType::Bracket) {
        throw std::runtime_error("Invalid bracket token");
    }
    const auto* closing = static_cast<BracketToken*>(bracket.get());

    // Карта для сопоставления закрывающих и открывающих скобок
    static const std::unordered_map<char, char> bracketPairs = {
        {')', '('},
        {']', '['},
        {'}', '{'}
    };

    // Проверяем, известна ли закрывающая скобка
    const auto it = bracketPairs.find(closing->getBracket());
    if (it == bracketPairs.end()) {
        throw std::runtime_error("Unknown closing bracket: " + std::string(1, closing->getBracket()));
    }
    const char expectedOpenBracket = it->second;

    // Поиск соответствующей открывающей скобки в стеке
    while (!stack.empty() && stack.top()->getType() != Token::TokenType::Bracket) {
        output.push_back(std::move(stack.top()));
        stack.pop();
    }

    if (stack.empty()) {
        throw std::runtime_error("Mismatched brackets: no opening bracket for " + std::string(1, closing->getBracket()));
    }

    if (const auto* openBracket = static_cast<BracketToken*>(stack.top().get()); openBracket->getBracket() != expectedOpenBracket) {
        throw std::runtime_error(
            "Mismatched bracket types: expected '" + std::string(1, expectedOpenBracket) +
            "', but got '" + std::string(1, openBracket->getBracket()) + "'"
        );
    }

    stack.pop(); // Удаляем открывающую скобку из стека

    // Если после скобки была функция, добавляем ее в вывод
    if (!stack.empty() && stack.top()->getType() == Token::TokenType::Function) {
        output.push_back(std::move(stack.top()));
        stack.pop();
    }
}

// Приоритет операторов
int Parser::getPriority(const Token& token) const {
    if (token.getType() == Token::TokenType::Operator) {
        const auto& op = static_cast<const OperatorToken&>(token);
        if (op.getOperator() == '^' || op.getOperator() == '!') return 4;
        if (op.getOperator() == '*' || op.getOperator() == '/') return 3;
        return 2; // +, -
    }
    if (token.getType() == Token::TokenType::Function) return 4;
    return 0;
}

// Ассоциативность
bool Parser::isRightAssociative(const Token& token) const {
    if (token.getType() == Token::TokenType::Operator) {
        const auto& op = static_cast<const OperatorToken&>(token);
        return (op.getOperator() == '^');
    }
    return false;
}