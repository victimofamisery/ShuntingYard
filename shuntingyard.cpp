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
    std::vector<std::string> functionsVector({"sin", "cos", "asin", "acos", "tg", "atg", "ctg", "ln"});
    std::vector<std::string> unaryVector({"sin", "cos", "asin", "acos", "tg", "atg", "ctg", "ln"});
    std::vector<std::string> rightAssociativityVector({"^"});

    double Pi = 3.14159265359;

    double fabs(double x) {
        if(x < 0) {
            return -x;
        }
        else {
            return x;
        }
    }

    double sin(double x){
        int i = 1;
        double cur = x;
        double acc = 1;
        double fact= 1;
        double pow = x;
        while (fabs(acc) > .00000001 &&   i < 100){
            fact *= ((2*i)*(2*i+1));
            pow *= -1 * x*x; 
            acc =  pow / fact;
            cur += acc;
            i++;
        }
        return cur;
    }

    double cos(double x) {
        return sin(2 * x) / (2 * sin(x));
    }

    double asin(double x) {
        if(fabs(x) > 1) {
            throw std::invalid_argument("Incorrect value in asin");
        }
        double i = 1;
        double cur = x;
        double acc = 1;
        double fact= 1;
        double pow = x;
        while (fabs(acc) > .00000001 &&   i < 100) {
            fact *= ((2*i-1) / (2*i) * (2*i - 1) / (2*i + 1));
            pow *= x*x; 
            acc =  pow * fact;
            cur += acc;
            i++;
        }
        return cur;
    }

    double acos(double x) {
        if(fabs(x) > 1) {
            throw std::invalid_argument("Incorrect value in acos");
        }
        return (Pi/2 - asin(x));
    }

    double tg(double x) {
        if(cos(x) == 0) {
            throw std::invalid_argument("Incorrect value in tg");
        }
        return sin(x) / cos(x);
    }

    double ctg(double x) {
        if(sin(x) == 0) {
            throw std::invalid_argument("Incorrect value in ctg");
        }
        return cos(x) / sin(x);
    }

    double atg(double x){
        return asin(x / pow(1 + pow(x, 1/2), 1/2)); 
        // // return atan(x);
        // int i = 1;
        // double cur = x;
        // double acc = 1;
        // double fact= 1;
        // double pow = x;
        // while (fabs(acc) > .00000001 &&   i < 100){
        //     fact = 2 * i + 1;
        //     pow *= -1 * x*x; 
        //     acc =  pow / fact;
        //     cur += acc;
        //     i++;
        // }
        // return cur;
    }

    double pow(double a, double b) {
        int i = 1;
        double cur = 1;
        double acc = 1;
        double fact= 1;
        double pow = 1;
        while (fabs(acc) > .00000001 &&   i < 100){
            fact *= log(a) / i;
            pow *= b; 
            acc =  pow * fact;
            cur += acc;
            i++;
        }
        return cur;
    }

    double ln(double x) {
        int i = 2;
        double cur = 2 * (x - 1)/(x + 1);
        double acc = 1;
        double fact= 1;
        double pow = 2 * (x - 1)/(x + 1);
        while (fabs(acc) > .00000001 &&   i < 100){
            fact = 2 * i - 1;
            pow *= (x - 1)/(x + 1) * (x - 1)/(x + 1); 
            acc =  pow / fact;
            cur += acc;
            i++;
        }
        return cur;
    }    

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
        else if (operation == "asin") {
            return asin(operand);
        }
        else if (operation == "acos") {
            return acos(operand);
        }
        else if (operation == "tg") {
            return tg(operand);
        }
        else if (operation == "atg") {
            return atg(operand);
        }
        else if (operation == "ctg") {
            return ctg(operand);
        }
        else if (operation == "ln") {
            return ln(operand);
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


std::vector<std::string> tokenize(const std::string& expression) {
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
                else if (*(&tokenVector.back() - 1) == "(") {
                    tokenVector.back() += c;
                }
                else {
                    tokenVector.push_back(std::string(&c, 1));
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
                tokenVector.back() == ")" || tokenVector.back() == "(" || isOperator(tokenVector.back())) 
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


std::queue<std::string> shuntingYard(const std::vector<std::string>& tokenVector) {
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


double calculate(std::queue<std::string> output) {
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

double ShuntingYard::calc(const std::string& expression) {
    auto tokens = tokenize(expression);
    auto output = shuntingYard(tokens);
    double result = calculate(output);
    return result;
}