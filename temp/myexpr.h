#ifndef MYEXPR_H
#define MYEXPR_H

#include "functionTable.hpp"
#include "variableTable.hpp"
#include <functional>
#include <memory>
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

class Lexer {
public:
  Lexer(const std::string &expr);
  auto nextToken() -> Token;

  void skipWhiteSpace();
  auto peek() -> char; // 查看字符,不位移
  auto get() -> char;  // 获取字符并位移

  auto get_pos() -> size_t;
  auto get_expression() -> std::string;

private:
  auto is_end() -> bool;
  auto peek_is_legal_number_type() -> bool;
  auto peek_is_legal_function_or_variable_type() -> bool;
  auto current_is_operator(char current) -> bool;
  std::string expression_;
  size_t pos_ = 0;
  static const std::unordered_set<std::string> functions;
};

enum class NodeType { Constant, Variable, BinaryOp, UnaryOp, Function };

class ExprNode {
public:
  ExprNode(ExprNode &others) = default;
  ExprNode(NodeType constant_type, double value);
  ExprNode(NodeType variable_type, std::string &name);
  ExprNode(NodeType unary_type, char operator_,
           std::shared_ptr<ExprNode> child);
  ExprNode(NodeType binary_type, char operator_,
           std::shared_ptr<ExprNode> left_child,
           std::shared_ptr<ExprNode> right_child);
  ExprNode(NodeType one_child_type, std::string &name,
           std::shared_ptr<ExprNode> child);
  ExprNode(NodeType two_childs_type, std::string &names,
           std::shared_ptr<ExprNode> left_child,
           std::shared_ptr<ExprNode> right_child);
  ExprNode(NodeType multi_childs_type, std::string &name,
           std::vector<std::shared_ptr<ExprNode>>);

  auto evaluate(VariableTable &varTable, FunctionTable &functable) -> double;

  auto get_unaryOp() -> std::function<double(double)>;
  void set_unaryOp(std::function<double(double)> unaryOp);

  auto get_binaryOp() -> std::function<double(double, double)>;
  void set_binaryOp(std::function<double(double, double)> binaryOp);

  void add_child(std::shared_ptr<ExprNode> child);

  auto get_arguments() -> const std::vector<std::shared_ptr<ExprNode>> &;

private:
  std::variant<double, std::string> value_;
  NodeType type_;

  std::function<double(double)> unaryOp_;
  std::function<double(double, double)> binaryOp_;
  std::vector<std::shared_ptr<ExprNode>> children_;
};

class Parser {
public:
  Parser(const std::string &lexpr);

  auto parse() -> std::shared_ptr<ExprNode>; // 解析方法

  auto parseExpression() -> std::shared_ptr<ExprNode>; // 实现递归下降解析器

  auto parseTerm() -> std::shared_ptr<ExprNode>; // 实现对乘除法操作的解析

  auto parseFactor() -> std::shared_ptr<ExprNode>; // 实现对幂运算的解析

  auto parseBase() -> std::shared_ptr<ExprNode>;
  // 实现对基础元素，如数字，变量，函数调用等

private:
  void nextToken();
  Lexer lexer_;
  Token current_token_;
};

class Evaluator {
public:
  Evaluator(std::shared_ptr<ExprNode> root_node);
  auto evaluate(VariableTable &varTable, FunctionTable &funcTable) -> double;

private:
  std::shared_ptr<ExprNode> root_;
};
/*
Lexer：词法分析器负责将输入的字符串分解为Token。
Parser：递归下降解析器通过调用 Lexer::nextToken()
生成Token流，逐步解析表达式并生成抽象语法树。
ExprNode：表达式树中的节点。每个节点代表表达式的一部分，包括常量、变量、运算符和函数。树的结构反映了表达式的计算顺序。
Evaluator：在语法树生成后，求值器负责递归地遍历语法树并计算结果。
 * */
} // namespace myexpr

#endif
