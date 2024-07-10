#include <iostream>
#include "myexpr.h"

void test_te_interp(const std::string& expression, double expected) {
    int error;
    double result = te_interp(expression, &error);
    if (error != 0) {
        std::cerr << "Error in expression: " << expression << " at position " << error << std::endl;
    } else if (std::fabs(result - expected) < 1e-6) {
        std::cout << "Expression: " << expression << " = " << result << " (PASS)" << std::endl;
    } else {
        std::cerr << "Expression: " << expression << " = " << result << ", expected " << expected << " (FAIL)" << std::endl;
    }
}

int main() {
    test_te_interp("3 + 5", 8.0);
    test_te_interp("10 - 2", 8.0);
    test_te_interp("2 * 3", 6.0);
    test_te_interp("8 / 4", 2.0);
    test_te_interp("2 ^ 3", 8.0);
    return 0;
}
