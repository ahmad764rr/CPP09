#include "RPN.hpp"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Error" << std::endl;
        return 1;
    }
    
    RPN calculator;
    calculator.calculate(argv[1]);
    
    return 0;
}
