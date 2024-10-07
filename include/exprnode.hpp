#ifndef EXPRNODE_HPP
#define EXPRNODE_HPP

#include "./functionTable.hpp"
#include "./variableTable.hpp"
#include <cassert>
#include <cctype>
#include <cmath>
#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>
#include <variant>

namespace myexpr {

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

} // namespace myexpr
#endif
