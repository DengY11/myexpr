#include <iostream>
#include "myexpr.h"
#include "myexpr.cpp"

void test_next_token(const std::string& expression) {
    State s;
    s.start = expression;
    s.next = s.start.begin();
    s.lookup = {};

    while (s.next != s.start.end()) {
        next_token(s);
        std::cout << "Token type: " << s.type << ", value: " << s.value << std::endl;
    }
}

int main() {
    test_next_token("3 + 5");
    test_next_token("10 - 2");
    test_next_token("2 * 3");
    test_next_token("8 / 4");
    test_next_token("2 ^ 3");
    return 0;
}
