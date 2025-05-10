//
// Created by Богдан on 10.05.2025.
//

#include "evaluator.h"
#include <cmath>

Evaluator::Evaluator(std::vector<std::unique_ptr<Token>> rpn, std::map<std::string, double> vars)
    : rpn(std::move(rpn)), variables(std::move(vars)) {}

double Evaluator::evaluate() {
    for (const auto& token : rpn) {
        processToken(*token);
    }
    if (calculationStack.size() != 1) {
        throw std::runtime_error("Invalid expression");
    }
    return calculationStack.top();
}

void Evaluator::processToken(const Token& token) {
    switch (token.getType()) {
        case Token::TokenType::Number:
            handleNumber(static_cast<const NumberToken&>(token));
            break;
        case Token::TokenType::Variable:
            handleVariable(static_cast<const VariableToken&>(token));
            break;
        case Token::TokenType::Operator:
            handleOperator(static_cast<const OperatorToken&>(token));
            break;
        case Token::TokenType::Function:
            handleFunction(static_cast<const FunctionToken&>(token));
            break;
        default:
            throw std::runtime_error("Unknown token type");
    }
}

void Evaluator::handleNumber(const NumberToken& number) {
    calculationStack.push(number.getValue());
}

void Evaluator::handleVariable(const VariableToken& variable) {
    const auto it = variables.find(variable.getName());
    if (it == variables.end()) {
        throw std::runtime_error("Undefined variable: " + variable.getName());
    }
    calculationStack.push(it->second);
}

void Evaluator::handleOperator(const OperatorToken& op) {
    if (calculationStack.size() < 2) {
        throw std::runtime_error("Not enough operands for operator");
    }
    const double b = calculationStack.top(); calculationStack.pop();
    const double a = calculationStack.top(); calculationStack.pop();

    switch (op.getOperator()) {
        case '+': calculationStack.push(a + b); break;
        case '-': calculationStack.push(a - b); break;
        case '*': calculationStack.push(a * b); break;
        case '/':
            if (b == 0) throw std::runtime_error("Division by zero");
            calculationStack.push(a / b);
            break;
        case '^': calculationStack.push(pow(a, b)); break;
        default: throw std::runtime_error("Unknown operator");
    }
}

void Evaluator::handleFunction(const FunctionToken& func) {
    if (calculationStack.empty()) {
        throw std::runtime_error("Not enough operands for function");
    }
    const double arg = calculationStack.top(); calculationStack.pop();

    if (func.getFunctionName() == "sin") {
        calculationStack.push(sin(arg));
    } else if (func.getFunctionName() == "cos") {
        calculationStack.push(cos(arg));
    } else if (func.getFunctionName() == "!") {
        if (arg < 0) throw std::runtime_error("Factorial of negative number");
        int result = 1;
        for (int i = 1; i <= arg; ++i) result *= i;
        calculationStack.push(result);
    } else {
        throw std::runtime_error("Unknown function: " + func.getFunctionName());
    }
}