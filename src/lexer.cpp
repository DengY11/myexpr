#include "../include/lexer.hpp"
#include <cassert>
#include <cctype>
#include <cmath>
#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>
namespace myexpr {

Lexer::Lexer(const std::string &expression)
    : expression_(expression), pos_(0), inited_(true) {}

auto Lexer::is_end() -> bool {
  check_initialization();
  return this->pos_ >= this->expression_.length();
}

void Lexer::skipWhiteSpace() {
  check_initialization();
  while (!is_end() && std::isspace(expression_[pos_])) {
    ++pos_;
  }
}

auto Lexer::peek() -> char {
  check_initialization();
  if (!is_end()) {
    return expression_[pos_];
  }
  return '\0';
}

auto Lexer::get() -> char {
  check_initialization();
  if (!is_end()) {
    return expression_[pos_++];
  }
  return '\0';
}

auto Lexer::peek_is_legal_number_type() -> bool {
  check_initialization();
  return (std::isdigit(peek()) || peek() == '.');
}

auto Lexer::peek_is_legal_function_or_variable_type() -> bool {
  check_initialization();
  return (std::isalnum(peek()) || peek() == '_');
  // 函数名或变量名为字母数字下划线
}

auto Lexer::current_is_operator(char current) -> bool {
  check_initialization();
  return (current == '+' || current == '-' || current == '*' ||
          current == '/' || current == '^' || current == '(' ||
          current == ')' || current == ',');
}

auto Lexer::nextToken() -> Token {
  check_initialization();
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
    if (this->funcTable.exist(name)) {
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

void Lexer::add_function(
    std::string &name,
    std::function<double(const std::vector<double> &)> func) {
  this->funcTable.add(name, func);
}
void Lexer::add_variable(std::string &name, double val) {
  this->varTable.set(name, val);
}

auto Lexer::is_inited() -> bool { return this->inited_; }

void Lexer::check_initialization() {
  if (!this->is_inited()) {
    throw std::runtime_error("lexer needs a expression to initd befor using");
  }
}

void Lexer::init_with_expression(const std::string &expr) {
  this->expression_ = expr;
  this->pos_ = 0;
  this->inited_ = true;
}
} // namespace myexpr
