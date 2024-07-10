#include <iostream>
#include <vector>
#include <string>
#include "myexpr.h"

void test_expression(const std::string& expr, double expected) {
    int error;
    double result = te_interp(expr, &error);

    if (error != 0) {
        std::cerr << "Error in expression: " << expr << " at position " << error << std::endl;
    } else if (std::fabs(result - expected) < 1e-6) {
        std::cout << "Expression: " << expr << " = " << result << " (PASS)" << std::endl;
    } else {
        std::cerr << "Expression: " << expr << " = " << result << ", expected " << expected << " (FAIL)" << std::endl;
    }
}

int main() {
    std::vector<std::pair<std::string, double>> tests = {
            {"3 + 5", 8.0},
            {"10 - 2", 8.0},
            {"2 * 3", 6.0},
            {"8 / 4", 2.0},
            {"2 ^ 3", 8.0},
            {"fac(5)", 120.0},
            {"ncr(5, 3)", 10.0},
            {"npr(5, 3)", 60.0},
            {"pi()", 3.141592653589793},
            {"e()", 2.718281828459045},
    };

    for (const auto& test : tests) {
        test_expression(test.first, test.second);
    }

    return 0;
}
