#include "shuntingyard.h"
int main() {

    std::string expression;
    std::getline(std::cin, expression);
    auto result = ShuntingYard::calc(expression);
    std::cout << result << std::endl;
    return 0;
}