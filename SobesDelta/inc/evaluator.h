//
// Created by Богдан on 10.05.2025.
//

#pragma once

#include <map>
#include <memory>
#include <stack>
#include <stdexcept>
#include <vector>

#include "Token.h"

class Evaluator {
public:
    Evaluator(std::vector<std::unique_ptr<Token>> rpn, std::map<std::string, double> vars);
    double evaluate();

private:
    std::vector<std::unique_ptr<Token>> rpn;
    std::map<std::string, double> variables;
    std::stack<double> calculationStack;

    void processToken(const Token& token);
    void handleNumber(const NumberToken& number);
    void handleVariable(const VariableToken& variable);
    void handleOperator(const OperatorToken& op);
    void handleFunction(const FunctionToken& func);
};
