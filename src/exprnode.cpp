#include "../include/exprnode.hpp"
#include <cassert>
#include <cctype>
#include <cmath>
#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>
namespace myexpr {
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
} // namespace myexpr
