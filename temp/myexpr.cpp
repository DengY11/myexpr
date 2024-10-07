#include "myexpr.h"
#include "functionTable.hpp"
#include "variableTable.hpp"
#include <cassert>
#include <cctype>
#include <cmath>
#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>

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

ExprNode::ExprNode(NodeType constant_type, double value)
    : type_(constant_type), value_(value){};

ExprNode::ExprNode(NodeType variable_type, std::string &name)
    : type_(variable_type), value_(name) {}
ExprNode::ExprNode(NodeType one_child_type, std::string &name,
                   std::shared_ptr<ExprNode> child)
    : type_(one_child_type), value_(name) {
  children_.push_back(child);
}
ExprNode::ExprNode(NodeType two_childs_type, std::string &name,
                   std::shared_ptr<ExprNode> left_child,
                   std::shared_ptr<ExprNode> right_child)
    : type_(two_childs_type), value_(name) {
  children_.push_back(left_child);
  children_.push_back(right_child);
}
ExprNode::ExprNode(NodeType unary_type, char operator_,
                   std::shared_ptr<ExprNode> child)
    : type_(unary_type), value_(std::string(1, operator_)) {
  if (operator_ == '+') {
    unaryOp_ = [](double arg) -> double { return arg; };
  } else if (operator_ == '-') {
    unaryOp_ = [](double arg) -> double { return -1.0 * arg; };
  } else {
    throw std::runtime_error("invalid unary operator: " +
                             std::string(1, operator_));
  }
  children_.push_back(child);
}
ExprNode::ExprNode(NodeType binary_type, char operator_,
                   std::shared_ptr<ExprNode> left_child,
                   std::shared_ptr<ExprNode> right_child)
    : type_(binary_type), value_(std::string(1, operator_)) {
  if (operator_ == '+') {
    binaryOp_ = [](double first_arg, double second_arg) -> double {
      return static_cast<double>(first_arg + second_arg);
    };
  } else if (operator_ == '-') {
    binaryOp_ = [](double first_arg, double second_arg) -> double {
      return static_cast<double>(first_arg - second_arg);
    };
  } else if (operator_ == '*') {
    binaryOp_ = [](double first_arg, double second_arg) -> double {
      return static_cast<double>(first_arg * second_arg);
    };
  } else if (operator_ == '/') {
    binaryOp_ = [](double first_arg, double second_arg) -> double {
      return static_cast<double>(first_arg / second_arg);
    };
  } else if (operator_ == '^') {
    binaryOp_ = [](double first_arg, double second_arg) -> double {
      return static_cast<double>(std::pow(first_arg, second_arg));
    };
  }
  children_.push_back(left_child);
  children_.push_back(right_child);
}

auto ExprNode::evaluate(VariableTable &varTable,
                        FunctionTable &funcTable) -> double {
  switch (type_) {
  case myexpr::NodeType::Constant:
    return std::get<double>(value_);
  case myexpr::NodeType::Variable: {
    auto variable = varTable.get(std::get<std::string>(this->value_));
    return variable;
  }
    // throw std::runtime_error("Variable evaluation not implemented");
  case myexpr::NodeType::UnaryOp:
    return this->unaryOp_(this->children_[0]->evaluate(varTable, funcTable));
  case myexpr::NodeType::BinaryOp:
    return this->binaryOp_(this->children_[0]->evaluate(varTable, funcTable),
                           children_[1]->evaluate(varTable, funcTable));
  case myexpr::NodeType::Function: {
    auto func = funcTable.get(std::get<std::string>(this->value_));
    auto &args_pointer = this->get_arguments();
    std::vector<double> args;
    for (const auto &arg_pointer : args_pointer) {
      args.push_back(arg_pointer->evaluate(varTable, funcTable));
    }
    return func(args);
  }
    // throw std::runtime_error("Function evaluation not implemented");
  default:
    throw std::runtime_error("Unknown node type");
  }
}

auto ExprNode::get_unaryOp() -> std::function<double(double)> {
  return this->unaryOp_;
}

void ExprNode::set_unaryOp(std::function<double(double)> unaryOp) {
  this->unaryOp_ = unaryOp;
}

auto ExprNode::get_binaryOp() -> std::function<double(double, double)> {
  return this->binaryOp_;
}

void ExprNode::set_binaryOp(std::function<double(double, double)> binaryOp) {
  this->binaryOp_ = binaryOp;
}

void ExprNode::add_child(std::shared_ptr<ExprNode> child) {
  children_.push_back(child);
}

auto ExprNode::get_arguments()
    -> const std::vector<std::shared_ptr<ExprNode>> & {
  return this->children_;
}

Parser::Parser(const std::string &expr) : lexer_(expr) { this->nextToken(); }

void Parser::nextToken() { this->current_token_ = this->lexer_.nextToken(); }

auto Parser::parseExpression() -> std::shared_ptr<ExprNode> {
  auto left_node = this->parseTerm();

  while (current_token_.get_type() == TokenType::Operator &&
         (current_token_.get_symbol() == '+' ||
          current_token_.get_symbol() == '-')) {
    auto op = current_token_.get_symbol();
    this->nextToken();
    auto right_node = this->parseTerm();

    auto binary_node = std::make_shared<ExprNode>(NodeType::BinaryOp, op,
                                                  left_node, right_node);
    left_node = binary_node;
  }
  return left_node;
}

auto Parser::parseTerm() -> std::shared_ptr<ExprNode> {
  auto left_node = parseFactor();
  while (current_token_.get_type() == TokenType::Operator &&
             (current_token_.get_symbol() == '*') ||
         current_token_.get_symbol() == '/') {
    char op = current_token_.get_symbol();
    nextToken();
    auto right_node = parseFactor();

    auto binary_node = std::make_shared<ExprNode>(NodeType::BinaryOp, op,
                                                  left_node, right_node);
    left_node = binary_node;
  }
  return left_node;
}

auto Parser::parseFactor() -> std::shared_ptr<ExprNode> {
  if (current_token_.get_type() == TokenType::Number) {
    double value = current_token_.get_number();
    nextToken();
    return std::make_shared<ExprNode>(NodeType::Constant, value);
  } else if (current_token_.get_type() == TokenType::Variable) {
    std::string name = current_token_.get_name();
    nextToken();
    return std::make_shared<ExprNode>(NodeType::Variable, name);
  } else if (current_token_.get_type() == TokenType::Function) {
    std::string name = current_token_.get_name();
    nextToken();

    std::vector<std::shared_ptr<ExprNode>> arguments;
    if (current_token_.get_symbol() == '(') {
      nextToken();
      while (current_token_.get_type() != TokenType::End &&
             current_token_.get_symbol() != ')') {
        arguments.push_back(parseFactor());
        if (current_token_.get_symbol() == ',') {
          nextToken();
        }
      }
      if (current_token_.get_symbol() == ')') {
        nextToken();
      }
    }
    return std::make_shared<ExprNode>(NodeType::Function, name, arguments);
  } else if (current_token_.get_symbol() == '(') {
    nextToken();
    auto node = parseExpression();
    if (current_token_.get_symbol() == ')') {
      nextToken();
    } else {
      throw std::runtime_error("Missing closing parenthesis");
    }
    return node;
  }
  throw std::runtime_error("Unknown factor");
}

Evaluator::Evaluator(std::shared_ptr<ExprNode> root) : root_(root) {}

auto Evaluator::evaluate(VariableTable &varTable,
                         FunctionTable &funcTable) -> double {
  return root_->evaluate(varTable, funcTable);
}

} // namespace myexpr
