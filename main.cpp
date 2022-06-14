#include "shuntingyard.h"

int main() {
    std::string expression;
    std::getline(std::cin, expression);
    auto tokens = ShuntingYard::tokenize(expression);
    auto output = ShuntingYard::shuntingYard(tokens);
    auto result = ShuntingYard::calculate(output);
    std::cout << result << std::endl;
    return 0;
}
