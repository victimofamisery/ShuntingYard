#include "shuntingyard.h"

int main() {

    // testing::InitGoogleTest(&argc, argv);
    std::string expression;
    std::getline(std::cin, expression);
    auto tokens = ShuntingYard::tokenize(expression);
    for(int i = 0; i <tokens.size(); i++){
        std::cout << tokens[i] << ' ';
    }
    std::cout << std::endl;
    auto output = ShuntingYard::shuntingYard(tokens);
    auto result = ShuntingYard::calculate(output);
    std::cout << result << std::endl;
    return 0;
}