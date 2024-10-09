#ifndef EXPR_CALCULATOR_HPP
#define EXPR_CALCULATOR_HPP

#include "./evaluator.hpp"
#include "./exprnode.hpp"
#include "./functionTable.hpp"
#include "./lexer.hpp"
#include "./parser.hpp"
#include "./token.hpp"
#include "./variableTable.hpp"
#include <unordered_map>

namespace myexpr {
using FunctionType = std::function<double(const std::vector<double> &)>;
using FunctionMap = std::unordered_map<std::string, FunctionType>;
using VariableMap = std::unordered_map<std::string, double>;

class expr_calculator {
public:
  // TODO::expr_calculator();
  expr_calculator(std::string &expression);
  expr_calculator(std::string &expression, FunctionMap &function_map);
  expr_calculator(std::string &expression, VariableMap &variable_map);
  expr_calculator(std::string &expression, VariableMap &variable_map,
                  FunctionMap &function_map);
  expr_calculator(std::string &expression, FunctionMap &function_map,
                  VariableMap &variable_map);
  void calculate();
  auto get_result() -> double;

private:
  myexpr::Parser paser_;
  double result_;
};

} // namespace myexpr

#endif // !EXPR_CALCULATOR_HPP
