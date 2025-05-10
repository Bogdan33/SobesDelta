//
// Created by Богдан on 10.05.2025.
//

#pragma once
#include <memory>
#include <stack>
#include <vector>
#include "Token.h"

class Parser {
public:
    explicit Parser(std::vector<std::unique_ptr<Token>> tokens);
    std::vector<std::unique_ptr<Token>> parse();

private:
    std::vector<std::unique_ptr<Token>> tokens_;
    size_t pos = 0;

    std::vector<std::unique_ptr<Token>> output;
    std::stack<std::unique_ptr<Token>> stack;

    void handleOperator(std::unique_ptr<Token> op);
    void handleFunction(std::unique_ptr<Token> func);
    void handleBracket(std::unique_ptr<Token> bracket);
    [[nodiscard]] int getPriority(const Token& token) const;
    [[nodiscard]] bool isRightAssociative(const Token& token) const;
};
