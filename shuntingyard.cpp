#include "shuntingyard.h"

std::vector<std::vector<std::string>> priorityVector({{"^"}, {"*", "/"}, {"+", "-"}});

std::vector<std::string> functionsVector({"sin", "cos"});

std::vector<std::string> unaryVector({"sin", "cos", "^"});

std::vector<std::string> rightAssociativityVector({"^"});

enum class Associativity {
    Left,
    Right
};

enum class Unarity  {
    Unary,
    Binary
};

std::vector<std::string> ShuntingYard::tokenize(const std::string& expression) {
    std::vector<std::string> tokenVector;
    tokenVector.push_back("");
    for (auto && c : expression) {
        if (c == ' ') {
            tokenVector.push_back("");
        }
        else if (c == '(') {
            tokenVector.back() = "(";
            tokenVector.push_back("");
        }
        else if (c == ')') {
            tokenVector.push_back(")");
        }
        else {
            tokenVector.back() += c;
        }
        for (auto&& token : tokenVector) {
            std::cout << token << std::endl;
        }
    }
    return tokenVector;
}

bool isNumber(const std::string& token)  {
    int count = 0;
    if (*token.begin() == '.') {
        return false;
    }
    if (*token.end() == '.') {
        return false;
    }
    for (auto&& c : token) {
        if (std::isdigit(c) == 0 && c != '.') {
            return false;
        }
        if ( c == '.') {
            count++;
        }
    }
    if (count > 1) {
        return false;
    }
    return true;
}

bool isFunction(const std::string& token) {
    for (auto function : functionsVector) {
        if (token == function) {
            return true;
        }
    }
    return false;
}

bool isOperator(const std::string& token) {
    for (auto&& priorityLevel : priorityVector) {
        for (auto&& operation : priorityLevel) {
            if (token == operation) {
                return true;
            }       
        }
    }
    return false;
}


ptrdiff_t priority(const std::string& token) {
    for (auto&& it = priorityVector.begin(); it < priorityVector.end(); it++) {
        if (std::find((*it).begin(), (*it).end(), token) != (*it).end()){
            return it - priorityVector.begin();
        }
    }
    return -1;
}

Associativity associativity(const std::string& token) {
    if (token == "^") {
        return Associativity::Right;
    }
    else {
        return Associativity::Left;
    }
}

std::queue<std::string> ShuntingYard::shuntingYard(const std::vector<std::string>& tokenVector) {
    std::stack<std::string> stack;
    std::queue<std::string> queue;
    for(auto&& token : tokenVector) {
        if (isNumber(token)) {
            queue.push(token);
        }
        else if (isFunction(token)) {
            stack.push(token);
        }
        else if (isOperator(token)) {
            while 
            (
                !stack.empty() &&
                stack.top() != "(" &&
                (
                    isFunction(stack.top()) ||
                    priority(stack.top()) < priority(token) ||
                    (
                        priority(stack.top()) == priority(token) &&
                        associativity(token) == Associativity::Left
                    )
                )
            )
            {
                queue.push(stack.top());
                stack.pop();
            }
            stack.push(token);
        }
        else if (token == "(") {
            stack.push(token);
        }
        else if (token == ")") {
            while (!stack.empty() && stack.top() != "(") {
                queue.push(stack.top());
                stack.pop();
            }
            if (stack.empty()) {
                std::string error = "Inbalanced parentheses";
                throw error;
            }
            stack.pop();
        }
        else {
            std::string error = "Wrong symbol";
            throw error;
        }
    }
    while (!stack.empty()) {
        if (stack.top() == "(") {
            std::string error = "Inbalanced parentheses";
            throw error; 
        }
        queue.push(stack.top());
        stack.pop();
    }
    return queue;
}

Unarity unarity(const std::string& token) {
    for (auto operators : unaryVector) {
        if (token == operators) {
            return Unarity::Unary;
        }
    }
    return Unarity::Binary;
}

double unary(std::string operation, double operand) {
    if (operation == "sin") {
        return sin(operand);
    }
    else if (operation == "cos") {
        return cos(operand);
    }
    throw "no operation for operand";
}

double binary(std::string operation, double operand1, double operand2) {
    if (operation == "+") {
        return operand1 + operand2;
    }
    else if (operation == "-") {
        return operand1 - operand2;
    }
    else if (operation == "*") {
        return operand1 * operand2;
    }
    else if (operation == "/") {
        return operand1 / operand2;
    }
    else if (operation == "^") {
        return pow(operand1, operand2);
    }
    throw "no operation for operand";
}

double ShuntingYard::calculate(std::queue<std::string> output) {
    std::stack<double> stack;
    while (!output.empty()) {
        auto token = output.front();
        output.pop();
        if (isNumber(token)) {
            stack.push(std::stod(token));
        }
        if (isOperator(token) || isFunction(token)) {
            if (unarity(token) == Unarity::Unary) {
                double operand = stack.top();
                stack.pop();
                stack.push(unary(token, operand));
            }
            else {
                double operand1 = stack.top();
                stack.pop();
                double operand2 = stack.top();
                stack.pop();
                stack.push(binary(token, operand1, operand2));
            }
        }
    }
    auto result = stack.top();
    stack.pop();
    if(stack.empty()) {
        return result;
    }
    else {
        std::string error = "Incorrect reverse polish notation";
        throw error;
    }
}

