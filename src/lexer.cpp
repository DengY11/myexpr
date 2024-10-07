#include "../include/lexer.hpp"
#include <cassert>
#include <cctype>
#include <cmath>
#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>
namespace myexpr {
// NOTE::内置函数表
const std::unordered_set<std::string> Lexer::functions = {"sin", "cos",  "tan",
                                                          "log", "sqrt", "abs"};

Lexer::Lexer(const std::string &expression)
    : expression_(expression), pos_(0) {}

auto Lexer::is_end() -> bool {
  return this->pos_ >= this->expression_.length();
}

void Lexer::skipWhiteSpace() {
  while (!is_end() && std::isspace(expression_[pos_])) {
    ++pos_;
  }
}

auto Lexer::peek() -> char {
  if (!is_end()) {
    return expression_[pos_];
  }
  return '\0';
}

auto Lexer::get() -> char {
  if (!is_end()) {
    return expression_[pos_++];
  }
  return '\0';
}

auto Lexer::peek_is_legal_number_type() -> bool {
  return (std::isdigit(peek()) || peek() == '.');
}

auto Lexer::peek_is_legal_function_or_variable_type() -> bool {
  return (std::isalnum(peek()) || peek() == '_');
  // 函数名或变量名为字母数字下划线
}

auto Lexer::current_is_operator(char current) -> bool {
  return (current == '+' || current == '-' || current == '*' ||
          current == '/' || current == '^' || current == '(' ||
          current == ')' || current == ',');
}

auto Lexer::nextToken() -> Token {
  skipWhiteSpace();

  if (pos_ >= expression_.length()) {
    return Token(TokenType::End);
  }

  // 是数字
  char current = get();
  if (std::isdigit(current) || current == '.') {
    std::string numberStr(1, current);
    while (pos_ < expression_.length() && peek_is_legal_number_type()) {
      numberStr += get();
    }
    return Token(TokenType::Number, std::stod(numberStr));
  }

  // 函数名或者变量名
  if (std::isalpha(current)) {
    std::string name(1, current);
    while (pos_ < expression_.length() &&
           peek_is_legal_function_or_variable_type()) {
      name += get();
    }
    if (this->functions.find(name) != functions.end()) {
      return Token(TokenType::Function, name);
    } else {
      return Token(TokenType::Variable, name);
    }
  }

  // 操作符
  if (current_is_operator(current)) {
    return Token(TokenType::Operator, current);
  }

  return Token(TokenType::Error);
}

} // namespace myexpr
