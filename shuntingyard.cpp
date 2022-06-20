#include "shuntingyard.h"

namespace {
    enum class Associativity {
        Left,
        Right
    };

    enum class Unarity  {
        Unary,
        Binary
    };


    std::vector<std::vector<std::string>> priorityVector({{"^"}, {"*", "/"}, {"+", "-"}});
    std::vector<std::string> functionsVector({"sin", "cos"});
    std::vector<std::string> unaryVector({"sin", "cos"});
    std::vector<std::string> rightAssociativityVector({"^"});


    bool isNumber(std::string token)  {
        int count = 0;
        if (token.empty()) {
            return false;
        }

        if (token[0] == '-' && token.length() == 1) {
            return false;
        }

        size_t start = 0;

        if(token[0] == '-') {
            start = 1;
        }

        for (auto i = start; i < token.length(); i++) {
            if (!std::isdigit(token[i]) && token[i] != '.') {
                return false;
            }
            if ( token[i] == '.') {
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


    static ptrdiff_t priority(const std::string& token) {
        for (auto&& it = priorityVector.begin(); it < priorityVector.end(); it++) {
            if (std::find((*it).begin(), (*it).end(), token) != (*it).end()) {
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


    Unarity unarity(const std::string& token) {
        for (auto operators : unaryVector) {
            if (token == operators) {
                return Unarity::Unary;
            }
        }

        return Unarity::Binary;
    }


    double unary(const std::string& operation, double operand) {
        if (operation == "sin") {
            return sin(operand);
        }

        else if (operation == "cos") {
            return cos(operand);
        }

        throw std::invalid_argument("no operation for operand");
    }


    double binary(const std::string& operation, double operand1, double operand2) {
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

        throw std::invalid_argument("no operation for operand");
    }
}


std::vector<std::string> ShuntingYard::tokenize(const std::string& expression) {
    std::vector<std::string> tokenVector;
    for (auto&& c : expression) {
        if (c == ' ') {  
            continue;   
        }

        else if (tokenVector.empty()) {
            tokenVector.push_back(std::string(&c, 1));
        }

        else if (std::isdigit(c) || c == '.') {
            if (isNumber(tokenVector.back())) {
                tokenVector.back() += c;
            }
            else if (tokenVector.back() == "-") {
                if (tokenVector.size() == 1) {
                    tokenVector.back() += c;
                }
                else if (!isNumber(*(&tokenVector.back() - 1))) {
                    tokenVector.back() += c;
                }
            } 
            else {
                tokenVector.push_back(std::string(&c, 1));
            }
        } 

        else if (c == ')' || c == '(') {
            tokenVector.push_back(std::string(&c, 1));
        }

        else {
            if (isNumber(tokenVector.back()) ||
                tokenVector.back() == ")" || tokenVector.back() == "(") 
            {
                tokenVector.push_back(std::string(&c, 1));
            }
            else {
                tokenVector.back() += c;
            }
        }
    }

    return tokenVector;
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
                throw std::invalid_argument("Inbalanced parentheses");
            }
            stack.pop();
        }

        else {
            throw std::invalid_argument("Wrong symbol");
        }
    }

    while (!stack.empty()) {
        if (stack.top() == "(") {
            throw std::invalid_argument("Inbalanced parentheses");
        }
        queue.push(stack.top());
        stack.pop();
    }

    return queue;
}


double ShuntingYard::calculate(std::queue<std::string> output) {
    std::stack<double> stack;
    while (!output.empty()) {
        auto token = output.front();
        output.pop();
        
        if (isNumber(token)) {
            stack.push(std::stod(token));
        }

        else if (isOperator(token) || isFunction(token)) {
            if (unarity(token) == Unarity::Unary) {
                double operand = stack.top();
                stack.pop();
                stack.push(unary(token, operand));
            }
            else {
                double operand2 = stack.top();
                stack.pop();
                double operand1 = stack.top();
                stack.pop();
                stack.push(binary(token, operand1, operand2));
            }
        }
    }

    auto result = stack.top();
    stack.pop();

    if (stack.empty()) {
        return result;
    }
    else {
        throw std::invalid_argument("Incorrect expression");
    }
}