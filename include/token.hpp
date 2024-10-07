#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "./functionTable.hpp"
#include "./variableTable.hpp"
#include <cmath>
#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <variant>

namespace myexpr {
enum class TokenType { Number, Operator, Function, Variable, End, Error };

class Token {

public:
  Token() = default;
  Token(TokenType end_or_error_type);
  Token(TokenType number_type, double val);
  Token(TokenType function_or_variable_type, std::string &name);
  Token(TokenType operator_type, char sym);
  Token(const Token &token) = default;
  Token(Token &&token) = default;

  auto get_type() -> TokenType;
  void set_type(TokenType token_type);

  auto get_number() const -> double;
  auto get_name() const -> std::string;
  auto get_symbol() const -> char;

  auto get_function() -> std::function<double(double, double)>;
  void set_function(std::function<double(double, double)>);

  auto operator=(const Token &other) -> Token &;
  auto operator=(const Token &&other) noexcept -> Token &;

private:
  TokenType token_type_;
  std::function<double(double, double)> binaryOp_ = nullptr;
  std::variant<double, std::string>
      value_; // 存储不同类型的值，数字是double，操作符是string，函数名或变量名也是string
};

} // namespace myexpr

#endif
