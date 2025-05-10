#include <iostream>

#include "CLI11.hpp"
#include "evaluator.h"
#include "lexer.h"
#include "parser.h"

int main(const int argc, char** argv)
{
    CLI::App app{"Calculator"};
    std::string expression;
    app.add_option("expression", expression, "Expression")->required();

    // Используем вектор для ручного парсинга переменных
    std::vector<std::string> var_args;
    app.add_option("--var", var_args, "Variables (e.g., x=3.14)");

    CLI11_PARSE(app, argc, argv);

    // Ручной парсинг переменных
    std::map<std::string, double> variables;
    for (const auto& arg : var_args) {
        size_t pos = arg.find('=');
        if (pos == std::string::npos) {
            std::cerr << "Error: Invalid variable format '" << arg << "'. Expected 'key=value'." << std::endl;
            return 1;
        }
        std::string key = arg.substr(0, pos);
        try {
            double value = std::stod(arg.substr(pos + 1));
            variables[key] = value;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error: Invalid value for variable '" << key << "': " << e.what() << std::endl;
            return 1;
        }
    }

    // Проверка данных (для отладки)
    std::cout << "Expression: " << expression << std::endl;
    for (const auto& [key, val] : variables)
    {
        std::cout << "Variable " << key << " = " << val << std::endl;
    }
    // Лексический анализ
    Lexer lexer(expression);
    auto tokens = lexer.tokenize();

    // Синтаксический анализ и преобразование в ОПН
    Parser parser(std::move(tokens));
    auto rpn = parser.parse();

    // Вычисление результата
    Evaluator evaluator(std::move(rpn), variables);
    double result = evaluator.evaluate();

    // Вывод результата
    std::cout << "Result: " << result << std::endl;
    return 0;
}