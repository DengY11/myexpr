#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP
#include "./exprnode.hpp"
#include "./functionTable.hpp"
#include "./variableTable.hpp"
#include <functional>
#include <memory>
#include <string>
#include <unordered_set>
#include <variant>
namespace myexpr {
class Evaluator {
public:
  Evaluator(std::shared_ptr<ExprNode> root_node);
  auto evaluate(VariableTable &varTable, FunctionTable &funcTable) -> double;

private:
  std::shared_ptr<ExprNode> root_;
};

} // namespace myexpr
#endif
