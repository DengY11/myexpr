#include <iostream>
#include "myexpr.h"

void test_te_compile(const std::string& expression) {
    int error;
    std::vector<TeExprType> variables; // 修改这里
    std::unique_ptr<TeExpr> expr = te_compile(expression, variables, &error);
    if (error != 0) {
        std::cerr << "Error in expression: " << expression << " at position " << error << std::endl;
    } else if (expr) {
        std::cout << "Expression: " << expression << " compiled successfully (PASS)" << std::endl;
    } else {
        std::cerr << "Expression: " << expression << " failed to compile (FAIL)" << std::endl;
    }
}


int main() {
    test_te_compile("3 + 5");
    test_te_compile("10 - 2");
    test_te_compile("2 * 3");
    test_te_compile("8 / 4");
    test_te_compile("2 ^ 3");
    return 0;
}
