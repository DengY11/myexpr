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
  Parser(const std::string &lexpr);

  auto parse() -> std::shared_ptr<ExprNode>; // 解析方法

  auto parseExpression() -> std::shared_ptr<ExprNode>; // 实现递归下降解析器

  auto parseTerm() -> std::shared_ptr<ExprNode>; // 实现对乘除法操作的解析

  auto parseFactor() -> std::shared_ptr<ExprNode>; // 实现对幂运算的解析

  // 实现对基础元素，如数字，变量，函数调用等

private:
  void nextToken();
  Lexer lexer_;
  Token current_token_;
};

} // namespace myexpr
#endif
