#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP
#include "./exprnode.hpp"
#include "./functionTable.hpp"
#include "./variableTable.hpp"
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <variant>
namespace myexpr {

template <typename T> struct is_VarTableType {
  static const bool value = std::is_same<T, VariableTable &>::value |
                            std::is_same<T, VariableTable>::value |
                            std::is_same<T, VariableTable &&>::value |
                            std::is_same<T, const VariableTable &>::value |
                            std::is_same<T, const VariableTable &&>::value |
                            std::is_same<T, const VariableTable>::value;
};

template <typename T> struct is_FuncTableType {
  static const bool value = std::is_same<T, FunctionTable &>::value |
                            std::is_same<T, FunctionTable &&>::value |
                            std::is_same<T, FunctionTable>::value |
                            std::is_same<T, const FunctionTable &>::value |
                            std::is_same<T, const FunctionTable &&>::value |
                            std::is_same<T, const FunctionTable>::value;
};

class Evaluator {
public:
  Evaluator(std::shared_ptr<ExprNode> root_node);

  template <
      typename VarTableType, typename FuncTableType,
      typename =
          typename std::enable_if<is_VarTableType<VarTableType>::value>::type,
      typename =
          typename std::enable_if<is_FuncTableType<FuncTableType>::value>::type>
  auto evaluate(VarTableType &&varTable, FuncTableType &&funcTable) -> double {
    return root_->evaluate(varTable, funcTable);
  }

private:
  std::shared_ptr<ExprNode> root_;
};

} // namespace myexpr
#endif
