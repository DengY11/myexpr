#include <iostream>
#include "myexpr.h"

void test_te_eval(const TeExpr* expr, double expected) {
    double result = te_eval(expr);
    if (std::fabs(result - expected) < 1e-6) {
        std::cout << "Expression evaluation result = " << result << " (PASS)" << std::endl;
    } else {
        std::cerr << "Expression evaluation result = " << result << ", expected " << expected << " (FAIL)" << std::endl;
    }
}

int main() {
    // Here you need to create TeExpr objects manually for testing
    // Example: test a simple value expression
    TeExpr value_expr(5.0);
    test_te_eval(&value_expr, 5.0);

    // Add more tests as needed for bound variables, functions, etc.
    return 0;
}
