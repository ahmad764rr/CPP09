#include "RPN.hpp"
#include <iostream>
#include <cstdlib>

RPN::RPN() {}

RPN::RPN(const RPN& other) {
    numbers = other.numbers;
}

RPN& RPN::operator=(const RPN& other) {
    if (this != &other) {
        numbers = other.numbers;
    }
    return *this;
}

RPN::~RPN() {}

static bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

static bool isDigitChar(char c) {
    return (c >= '0' && c <= '9');
}

void RPN::calculate(const std::string& expression) {
    size_t i = 0;
    bool hasOperation = false;
    
    while (i < expression.length()) {
        char current = expression[i];
        
        // Skip spaces
        if (current == ' ') {
            i++;
            continue;
        }
        
        // Check if it's a single digit
        if (isDigitChar(current)) {
            // Make sure it's a single digit (next char should be space or end)
            if (i + 1 < expression.length() && expression[i + 1] != ' ') {
                std::cerr << "Error" << std::endl;
                exit(1);
            }
            numbers.push(current - '0');
            i++;
            continue;
        }
        
        // Check if it's an operator
        if (isOperator(current)) {
            // Need at least 2 numbers for operation
            if (numbers.size() < 2) {
                std::cerr << "Error" << std::endl;
                exit(1);
            }
            
            hasOperation = true;
            
            int second = numbers.top();
            numbers.pop();
            int first = numbers.top();
            numbers.pop();
            
            int result = 0;
            if (current == '+') {
                result = first + second;
            } else if (current == '-') {
                result = first - second;
            } else if (current == '*') {
                result = first * second;
            } else if (current == '/') {
                if (second == 0) {
                    std::cerr << "Error" << std::endl;
                    exit(1);
                }
                result = first / second;
            }
            
            numbers.push(result);
            i++;
            continue;
        }
        
        // Invalid character
        std::cerr << "Error" << std::endl;
        exit(1);
    }
    
    // Should have exactly one number left and at least one operation
    if (numbers.size() != 1 || !hasOperation) {
        std::cerr << "Error" << std::endl;
        exit(1);
    }
    
    std::cout << numbers.top() << std::endl;
}
