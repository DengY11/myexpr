#ifndef LEXER_HPP
#define LEXER_HPP

#include "./functionTable.hpp"
#include "./token.hpp"
#include "./variableTable.hpp"
#include <cassert>
#include <cctype>
#include <cmath>
#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <variant>

namespace myexpr {

class Lexer {
public:
  Lexer() = default;
  Lexer(const std::string &expr);
  auto nextToken() -> Token;

  void skipWhiteSpace();
  auto peek() -> char; // 查看字符,不位移
  auto get() -> char;  // 获取字符并位移

  auto get_pos() -> size_t;
  auto get_expression() -> std::string;
  void add_function(std::string &name,
                    std::function<double(const std::vector<double> &)> func);
  void add_variable(std::string &name, double val);

  auto is_inited() -> bool;

  void init_with_expression(const std::string &expr);
  auto get_functable() -> FunctionTable &;

  auto get_vartable() -> VariableTable &;

private:
  void check_initialization();
  auto is_end() -> bool;
  auto peek_is_legal_number_type() -> bool;
  auto peek_is_legal_function_or_variable_type() -> bool;
  auto current_is_operator(char current) -> bool;
  auto current_is_equal(char current) -> bool;
  auto current_is_brackets(char current) -> bool;

  bool inited_ = false;
  std::string expression_;
  size_t pos_ = 0;
  FunctionTable funcTable;
  VariableTable varTable;
};

} // namespace myexpr

#endif // !LEXER_HPP
