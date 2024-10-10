#ifndef PARSER_HPP
#define PARSER_HPP

#include "./exprnode.hpp"
#include "./functionTable.hpp"
#include "./lexer.hpp"
#include "./token.hpp"
#include "./variableTable.hpp"
#include <functional>
#include <memory>
#include <string>
#include <unordered_set>
#include <variant>
namespace myexpr {

class Parser {
public:
  Parser() = default;

  Parser(const std::string &lexpr);

  auto parse() -> std::shared_ptr<ExprNode>; // 解析方法

  auto parse(const std::string &lexer) -> std::shared_ptr<ExprNode>;

  auto parseExpression() -> std::shared_ptr<ExprNode>; // 实现递归下降解析器

  auto parseTerm() -> std::shared_ptr<ExprNode>; // 实现对乘除法操作的解析

  auto parseFactor() -> std::shared_ptr<ExprNode>; // 实现对幂运算的解析

  // 实现对基础元素，如数字，变量，函数调用等
  void add_function_map(
      std::unordered_map<std::string,
                         std::function<double(const std::vector<double> &)>>
          function_map);
  void add_function(std::string &name,
                    std::function<double(const std::vector<double> &)> func);

  void add_variable_map(std::unordered_map<std::string, double> variable_map);
  void add_variable(std::string &name, double val);

  auto get_variable_table() -> const VariableTable &;
  auto get_function_table() -> const FunctionTable &;

  auto is_inited() -> bool;

private:
  void nextToken();
  Lexer lexer_;
  Token current_token_;
  bool inited_ = false;
  ;
};

} // namespace myexpr
#endif
