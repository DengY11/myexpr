#include "../include/parser.hpp"
#include <cassert>
#include <cctype>
#include <cmath>
#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>

namespace myexpr {
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

} // namespace myexpr
