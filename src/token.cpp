#include "../include/token.hpp"
#include <cassert>

namespace myexpr {
Token::Token(TokenType end_or_error_type) : token_type_(end_or_error_type) {
  assert(end_or_error_type == TokenType::End ||
         end_or_error_type == TokenType::Error);
}
Token::Token(TokenType number_type, double value)
    : token_type_(number_type), value_(value) {
  assert(number_type == TokenType::Number);
}
Token::Token(TokenType function_or_variable_type, std::string &name)
    : token_type_(function_or_variable_type), value_(name) {
  assert(function_or_variable_type == TokenType::Function ||
         function_or_variable_type == TokenType::Variable);
}
Token::Token(TokenType operator_type, char sym)
    : token_type_(operator_type), value_(std::string(1, sym)) {
  assert(operator_type == TokenType::Operator);
};

auto Token::get_type() -> TokenType { return this->token_type_; }
void Token::set_type(TokenType token_type) { this->token_type_ = token_type; }

auto Token::get_number() const -> double {
  assert(this->token_type_ == TokenType::Number);
  return std::get<double>(value_);
}

auto Token::get_name() const -> std::string {
  assert(this->token_type_ == TokenType::Function ||
         this->token_type_ == TokenType::Variable);
  return std::get<std::string>(value_);
}

auto Token::get_symbol() const -> char {
  assert(this->token_type_ == TokenType::Operator);
  return static_cast<char>(std::get<std::string>(value_)[0]);
}

auto Token::get_function() -> std::function<double(double, double)> {
  assert(this->binaryOp_ != nullptr &&
         this->token_type_ == TokenType::Function);
  return this->binaryOp_;
}
void Token::set_function(std::function<double(double, double)> binaryOp) {
  this->binaryOp_ = binaryOp;
}

auto Token::operator=(const Token &other) -> Token & {
  if (this != &other) {
    this->token_type_ = other.token_type_;
    this->value_ = other.value_;
  }
  return *this;
}

auto Token::operator=(const Token &&other) noexcept -> Token & {
  if (this != &other) {
    this->token_type_ = other.token_type_;
    this->value_ = std::move(other.value_);
  }
  return *this;
}
} // namespace myexpr
