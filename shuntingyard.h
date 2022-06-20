#pragma once
#include <string>
#include <queue>
#include <algorithm>
#include <iostream>
#include <stack>
#include <cmath>
#include <exception> 

namespace ShuntingYard{
    std::vector<std::string> tokenize(const std::string& expression);
    std::queue<std::string> shuntingYard(const std::vector<std::string>& tokenVector);
    double calculate(std::queue<std::string> output); 
}